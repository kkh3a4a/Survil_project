#include "Common.h"
#include"global.h"
#include <iostream>
#include <fstream>
#include<vector>
#include<mutex>
#include<string>
#include<map>
#include <chrono>
#include "map.cu"

#define SERVERPORT 9000
#define BUFSIZE    4096
using namespace std;
using namespace chrono;

//함수정의 
DWORD WINAPI ProcessClient(LPVOID arg);
DWORD WINAPI ingame_thread(LPVOID arg);


map<int, FActor*> players_list; //port, player_info
bool game_start = false;
int len = 0;

std::mutex mylock;
TF sun_angle;

vector<SOCKET> player_list;
map <FActor*, location_rotation>my_citizen;

DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval = 0;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	FActor testActor;
	game_start = true;
	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));
	auto start_t = high_resolution_clock::now();

	FActor player_info;
	//해당 클라이언트의 port번호 map에 저장
	int port = ntohs(clientaddr.sin_port);
	players_list[port] = &player_info;

	int cnt = 0;
	retval = recv(client_sock, (char*)&cnt, sizeof(int), 0);

	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 0;
	}
	for (int i = 0; i < cnt; ++i)
	{
		bool overlap = false;
		retval = recv(client_sock, (char*)&testActor, sizeof(testActor), 0);
		for (auto& a : my_citizen){
			if (wcscmp(a.first->name, testActor.name) == 0)
			{
				overlap = true;
			}
		}
		if(!overlap){
			FActor* tempActor = new FActor();
			wcscpy(tempActor->name, testActor.name);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				return 0;
			}
			FActor_TF_define(my_citizen[tempActor].location, testActor.location);
			FActor_TF_define(tempActor->location, testActor.location);
			FActor_TF_define(my_citizen[tempActor].rotation, testActor.rotation);
			FActor_TF_define(tempActor->rotation, testActor.rotation);
		}
		/*wcout << testActor.name;
		cout << " : " << testActor.location.x << ", " << testActor.location.y << endl;*/
	}

	while (1) {
		auto end_t = high_resolution_clock::now();
		if (duration_cast<milliseconds>(end_t - start_t).count() > 50) {
			start_t = high_resolution_clock::now();
			retval = recv(client_sock, (char*)&testActor, (int)sizeof(testActor), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			if (wcscmp(testActor.name, L"temp") != 0){
				for (auto& a : my_citizen){{
						FActor_TF_define(a.second.location, testActor.location);	
					}
				}
			}
			retval = send(client_sock, (char*)&sun_angle, (int)sizeof(TF), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			for (auto& a : my_citizen){
				retval = send(client_sock, (char*)&(*a.first), (int)sizeof(testActor), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
				wcout << a.first->name;
				cout << " : " << a.second.location.x << ", " << a.second.location.y << endl;
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

	while (1) {
		auto sunangle_end_t = high_resolution_clock::now();
		auto actor_move_end_t = high_resolution_clock::now();

		if (duration_cast<milliseconds>(sunangle_end_t - sunangle_start_t).count() > 50) {
			sunangle_start_t = high_resolution_clock::now();
			sun_angle.y += 0.2f;
			if (sun_angle.y >= 180.f) {
				sun_angle.y = -180.f;
			}
			for (auto& a : my_citizen){
				float distance = 0.0f;
				if (location_distance(a.first->location, a.second.location) > 10){
					Move_Civil(a.first->location, a.second.location);
				}
			}
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	

	Map map;
	map.get_device_info();

	//Terrain move & Player Sight Update===================================================
	//II player_location = { 0, 0 };		//이거 나중에 중심 기준으로 바꿔야함
	//int wind_angle = 270;		//각도
	//int wind_speed = 50;		//최대 풍속 50
	//for (int i = 0; i < 1; i++) {
	//	clock_t t_1 = clock();

	//	//Terrain Move
	//	wind_decide(wind_speed, wind_angle);

	//	FF wind_direction = { cos(wind_angle * PI / 180), sin(wind_angle * PI / 180) };
	//	if (abs(wind_direction.x) < FLT_EPSILON) {
	//		wind_direction.x = 0;
	//	}
	//	if (abs(wind_direction.y) < FLT_EPSILON) {
	//		wind_direction.y = 0;
	//	}

	//	move_terrain(hill_location_host, num_of_hills, wind_direction, wind_speed);
	//	if (num_of_hills < origin_num_of_hills) {
	//		make_new_hills(hill_location_host, num_of_hills, origin_num_of_hills, wind_direction, wind_speed);
	//	}

	//	cudaMemcpy(hill_location_device, hill_location_host, num_of_hills * sizeof(HI), cudaMemcpyHostToDevice); //Memcpy to Device

	//	//Player Sight Update
	//	//player_location.x += 20;
	//	//player_location.y += 20;
	//	//thread must be 1024 for efficiency
	//	player_terrain_update_cuda << <player_sight_size, player_sight_size >> > (terrain_player_sight_device, hill_location_device, num_of_hills, player_location, wind_direction, wind_speed);
	//	for (int i = 0; i < player_sight_size; i++) {
	//		cudaMemcpy(terrain_player_sight_host[i], terrain_player_sight_temp[i], player_sight_size * sizeof(char), cudaMemcpyDeviceToHost);
	//	}
	//	clock_t t_2 = clock();
	//	cout << "Player Sight Update Time : " << (double)(t_2 - t_1) / CLOCKS_PER_SEC << " Seconds" << endl;
	//	//show_array(terrain_player_sight_host, player_sight_size);
	//	cout << "==============================" << endl;
	//}
	//=========================================================================================
	
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
