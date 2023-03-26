#include <iostream>
#include <fstream>
#include<vector>
#include<mutex>
#include<string>
#include<map>
#include <chrono>
#include<shared_mutex>
#include<algorithm>
#include"Network.h"

#include "terrain.cu"


#pragma comment (lib,"WS2_32.lib")
#pragma comment (lib,"MSWSock.lib")


using namespace std;
using namespace chrono;

uniform_int_distribution <int>map_uid{ 1000, one_side_number - 1000 };
//함수정의 
DWORD WINAPI ProcessClient(LPVOID arg);
DWORD WINAPI ingame_thread(LPVOID arg);

bool game_start = false;
int len = 0;

std::mutex player_cnt_lock;
shared_mutex player_list_lock;
float sun_angle;

Terrain* terrain = new Terrain();
char** total_terrain = terrain->get_map();
char** shadow_map = terrain->get_shadow_map();
unsigned char** temperature_map = terrain->get_temperature_map();
volatile int player_cnt;
volatile bool location_set = false;
int ingame_play = false;

DWORD WINAPI terrain_change(LPVOID arg)
{
	/*char** player_sight_terrain = terrain->get_player_sight_map();
	char** player_sight_temperature = terrain->get_player_temperature_map();
	terrain->set_city_location(TF{ 20000, 20000 }, 0);*/

	//terrain->show_array(total_terrain, 320);
	terrain->log_on();
	int i{};
	while (1){
		//clock_t t_0 = clock();
		cout << endl << i << "번째" << endl;
		

		terrain->wind_blow({ 1, 0 }, 1);
		terrain->make_shadow_map(sun_angle);
		terrain->make_tempertature_map(sun_angle);
		CC retval = terrain->get_highest_lowest(temperature_map);

		/*if (sun_angle > 360)
			sun_angle = 0;
		sun_angle += 6;*/
		cout << "sun_angle : " << sun_angle << endl;
		cout << "Temperature Highest: " << (float)retval.x / 4 << ", Lowest" << (float)retval.y / 4 << endl;

		//terrain->show_array(total_terrain, 320);
		//terrain->show_array(shadow_map, 320);
		//terrain->show_array(temperature_map, 320);


		/*terrain->copy_for_player_map(II{ 200, 200 });
		terrain->show_array(player_sight_terrain, 120);
		terrain->show_array(player_sight_temperature, 120);*/


		//clock_t t_1 = clock();
		//cout << "[[[ Loop:" << (double)(t_1 - t_0) / CLOCKS_PER_SEC << " sec ]] ]" << endl;
		if (i % 100 == 0 && i != 0) {
			terrain->save_terrain();
			cout << "SAVED!!!" << endl;
		}
		i++;
	}
}

DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval = 0;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	
	return 0;
}

DWORD WINAPI ingame_thread(LPVOID arg)
{
	return 0;
}

int main(int argc, char* argv[])
{	
	cout << fixed;
	HANDLE hThread;
	//hThread = CreateThread(NULL, 0, terrain_change, 0, 0, NULL);

	// 윈속 초기화
	WSADATA wsa;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsa);

	// SOCKET 생성 listen & bind
	SOCKET s_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN s_address;
	ZeroMemory(&s_address, sizeof(s_address));
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(SERVERPORT);
	s_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	bind(s_socket, reinterpret_cast<sockaddr*>(&s_address), sizeof(s_address));
	listen(s_socket, SOMAXCONN);

	SOCKADDR_IN c_address;
	ZeroMemory(&c_address, sizeof(c_address));
	int c_addr_size = sizeof(c_address);

	h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(s_socket), h_iocp, 99999, 0);
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	WSA_OVER_EX accept_over(IOCPOP::OP_ACCEPT,0,0);


	// accept()
	AcceptEx(s_socket, c_socket, accept_over.getbuf(), NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accept_over.getWsaOver());

	//모든 object 초기화
	for (auto object : objects)
	{
		object = nullptr;
	}
	for (int i = 0; i < MAXPLAYER; ++i)
	{
		objects[i] = new Player();
	}
	for (int i = 0; i < MAXPLAYER; ++i)
	{
		default_random_engine dre2;
		dre2.seed(std::chrono::system_clock::now().time_since_epoch().count());
		retry:		
		float x = map_uid(dre) * UNIT, float y = map_uid(dre) * UNIT, float z = 0;
		for (int j = 0; j < i; ++j)
		{
			if (location_distance(objects[j]->_x, objects[j]->_y, x, y) < 7000)
			{
				cout << "retry" << endl;
				goto retry;
			}
		}
		reinterpret_cast<Player*>(objects[i])->set_player_location(x, y, z);
	}
	for (int i = 0; i < MAXPLAYER; ++i)
	{
		cout << objects[i]->_x << ", " << objects[i]->_y << endl;
	}



	while (1) {

	}
	
	return 0;
}
