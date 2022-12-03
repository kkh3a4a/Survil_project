#include "..\..\Common.h"
#include <iostream>
#include <fstream>
#include<vector>
#include<mutex>
#include<string>
#include<map>
#include <chrono>

#define SERVERPORT 9000
#define BUFSIZE    4096
using namespace std;
using namespace chrono;

//함수정의 
DWORD WINAPI ProcessClient(LPVOID arg);
DWORD WINAPI ingame_thread(LPVOID arg);

typedef struct three_float {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
}TF;

typedef struct transform {
	TF location;
	TF rotation;
}T;

map<int, T*> players_list; //port, player_info
bool game_start = false;
int len = 0;

std::mutex mylock;
TF sun_angle;
T testActor;
vector<SOCKET> player_list;

DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];
	
	game_start = true;
	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",addr, ntohs(clientaddr.sin_port));
	auto start_t = high_resolution_clock::now();
	
	T player_info;
	//해당 클라이언트의 port번호 map에 저장
	int port = ntohs(clientaddr.sin_port);
	players_list[port] = &player_info;
	
	while (1) {
		auto end_t = high_resolution_clock::now();
		if (duration_cast<milliseconds>(end_t - start_t).count() > 50){
			start_t = high_resolution_clock::now();
			retval = send(client_sock, (char*)&sun_angle, (int)sizeof(TF), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			retval = send(client_sock, (char*)&testActor, (int)sizeof(T), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			//cout << sun_angle.y << endl;
		}
	}
	
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		addr, ntohs(clientaddr.sin_port));
	// 소켓 닫기
	closesocket(client_sock);

	return 0;
}
DWORD WINAPI ingame_thread(LPVOID arg)
{
	/*sun_angle.x = 0.0f;
	sun_angle.y = 0.0f;
	sun_angle.z = 0.0f;
	testActor.location.x = 0.0f;
	testActor.location.y = 0.0f;
	testActor.location.z = 500.0f;
	testActor.rotation.x = 0.0f;
	testActor.rotation.y = 0.0f;
	testActor.rotation.z = 0.0f;*/
	
	auto sunangle_start_t = high_resolution_clock::now();
	auto actor_move_start_t = high_resolution_clock::now();
	
	while (1){
		auto sunangle_end_t = high_resolution_clock::now();
		auto actor_move_end_t = high_resolution_clock::now();
		
		if (duration_cast<milliseconds>(sunangle_end_t - sunangle_start_t).count() > 50){
			sunangle_start_t = high_resolution_clock::now();
			sun_angle.y += 0.2f;
			if (sun_angle.y >= 180.f) {
				sun_angle.y = -180.f;
			}
			
			testActor.location.x += testActor.rotation.x * 0.5;
			testActor.location.y += testActor.rotation.y * 0.5;
			//cout << testActor.location_x << ", " << testActor.location_y << endl;
		}
		if (duration_cast<milliseconds>(actor_move_end_t - actor_move_start_t).count() > 5000){
			actor_move_start_t = high_resolution_clock::now();
			testActor.rotation.x = rand() % 100;
			testActor.rotation.y = rand() % 100;
			if (rand() % 2 == 1) {
				testActor.rotation.x *= -1;
			}
			if (rand() % 2 == 1) {
				testActor.rotation.y *= -1;
			}
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	int retval;
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, ingame_thread,
		0, 0, NULL);

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

		cout << addr << endl;

		player_list.emplace_back(client_sock);
		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)client_sock, 0, NULL);
		//소켓 닫기
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }

	}

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
