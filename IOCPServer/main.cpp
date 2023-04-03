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
#include"Player.h"
#include"Citizen.h"
#include"Resource.h"

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
volatile int player_cnt = 0;


/////////////////////////////////







//Terrain* terrain = new Terrain();
//char** total_terrain = terrain->get_map();
//char** shadow_map = terrain->get_shadow_map();
//unsigned char** temperature_map = terrain->get_temperature_map();
//
//volatile bool location_set = false;
//int ingame_play = false;
//
//DWORD WINAPI terrain_change(LPVOID arg)
//{
//	/*char** player_sight_terrain = terrain->get_player_sight_map();
//	char** player_sight_temperature = terrain->get_player_temperature_map();
//	terrain->set_city_location(TF{ 20000, 20000 }, 0);*/
//
//	//terrain->show_array(total_terrain, 320);
//	terrain->log_on();
//	int i{};
//	while (1){
//		//clock_t t_0 = clock();
//		cout << endl << i << "번째" << endl;
//		
//
//		terrain->wind_blow({ 1, 0 }, 1);
//		terrain->make_shadow_map(sun_angle);
//		terrain->make_tempertature_map(sun_angle);
//		CC retval = terrain->get_highest_lowest(temperature_map);
//
//		/*if (sun_angle > 360)
//			sun_angle = 0;
//		sun_angle += 6;*/
//		cout << "sun_angle : " << sun_angle << endl;
//		cout << "Temperature Highest: " << (float)retval.x / 4 << ", Lowest" << (float)retval.y / 4 << endl;
//
//		//terrain->show_array(total_terrain, 320);
//		//terrain->show_array(shadow_map, 320);
//		//terrain->show_array(temperature_map, 320);
//
//
//		/*terrain->copy_for_player_map(II{ 200, 200 });
//		terrain->show_array(player_sight_terrain, 120);
//		terrain->show_array(player_sight_temperature, 120);*/
//
//
//		//clock_t t_1 = clock();
//		//cout << "[[[ Loop:" << (double)(t_1 - t_0) / CLOCKS_PER_SEC << " sec ]] ]" << endl;
//		if (i % 100 == 0 && i != 0) {
//			terrain->save_terrain();
//			cout << "SAVED!!!" << endl;
//		}
//		i++;
//	}
//}

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
	auto Player_Move_Timer_End = std::chrono::system_clock::now();
	auto Citizen_Move_Timer_End = std::chrono::system_clock::now();
	auto Resource_Collect_Timer_End = std::chrono::system_clock::now();
	while (1)
	{
		auto Timer_Start = std::chrono::system_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(Timer_Start - Player_Move_Timer_End).count() > 10)
		{
			Player_Move_Timer_End = std::chrono::system_clock::now();
			for (int i = 0; i < MAXPLAYER; ++i)
			{
				Player* player = reinterpret_cast<Player*>(objects[i]);
				if (player->isconnect)
				{
					player->keyinput();
				}
			}
		}
		if (std::chrono::duration_cast<std::chrono::milliseconds>(Timer_Start - Citizen_Move_Timer_End).count() > 10)
		{
			Citizen_Move_Timer_End = std::chrono::system_clock::now();
			for (int i = CITIZENSTART; i < MAXCITIZEN + CITIZENSTART; ++i)
			{
				Citizen* citizen = reinterpret_cast<Citizen*>(objects[i]);
				if (citizen->_Job == -1)
				{
					continue;
				}
				citizen->set_citizen_move();
				
			}
		}
		if (std::chrono::duration_cast<std::chrono::milliseconds>(Timer_Start - Resource_Collect_Timer_End).count() > 5000)
		{	
			Resource_Collect_Timer_End = std::chrono::system_clock::now();
			for (int i = RESOURCESTART; i < RESOURCESTART + MAXRESOURCE; ++i)
			{
				Resource* resource = reinterpret_cast<Resource*>(objects[i]);
				resource->collect_resource();
			}
			for (int i = 0; i < MAXPLAYER; ++i)
			{
				Player* player = reinterpret_cast<Player*>(objects[i]);
				player->send_resourceacount();
			}
		}

	}


	return 0;
}

int main(int argc, char* argv[])
{	
	cout << fixed;
	HANDLE hThread;
	//hThread = CreateThread(NULL, 0, terrain_change, 0, 0, NULL);
	hThread = CreateThread(NULL, 0, ingame_thread, 0, 0, NULL);
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

	h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(s_socket), h_iocp, 99999, 0);
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	WSA_OVER_EX accept_over;
	ZeroMemory(&accept_over, sizeof(accept_over));
	accept_over.set_accept_over();

	// accept()
	AcceptEx(s_socket, c_socket, accept_over.getbuf(), NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accept_over.getWsaOver());

	//모든 object 초기화
	for (auto object : objects)
	{
		object = nullptr;
	}

	//미리 object의 종류를 정해줌
	for (int i = 0; i < MAXPLAYER; ++i)
	{
		objects[i] = new Player(i);
	}
	for (int i = MAXPLAYER; i < MAXCITIZEN + MAXPLAYER; ++i)//5~1004까지는 Citizen
	{
		objects[i] = new Citizen(i);
	}
	for (int i = RESOURCESTART; i < RESOURCESTART + MAXRESOURCE; ++i)//1005 ~ 1054까지는 Resource
	{
		objects[i] = new Resource(i ,i % 5);	//i%5 해서 type을 정해줌 /0 : 석유,	1 : 물,		2 : 철,		3 : 식량,		4 : 나무
	}
	

	//player, citizen, resource 초기 위치 지정 한번 하고 안쓸거라 main에 둠
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
				goto retry;
			}
		}
		reinterpret_cast<Player*>(objects[i])->set_player_location(x, y, z);
		for (int j = 0; j < 10; ++j)
		{
			reinterpret_cast<Citizen*>(objects[MAXPLAYER + i * 200 + j])->set_citizen_spwan_location(x + 2000, y + (j * 500) - 2250,z);
			reinterpret_cast<Resource*>(objects[RESOURCESTART + i * 10 + j])->set_resource_spwan_location(x, y, z);
		}

	}


	int user_id = 0;

	while (1) {

		DWORD io_byte;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		ret = GetQueuedCompletionStatus(h_iocp, &io_byte, &key, &over, INFINITE);
		WSA_OVER_EX* wsa_over_ex = reinterpret_cast<WSA_OVER_EX*>(over);
		if (ret == FALSE)
		{
			if (wsa_over_ex->_iocpop == OP_ACCEPT) cout << "Accept Error";
			else {
				cout << "GQCS Error on client [" << key << "]\n";
				//disconnect(static_cast<int>(key));
				//if (ex_over->_comp_type == OP_SEND) delete ex_over;
				continue;
			}
		}

		user_id = static_cast<int>(key);
		Player* player;

		if (user_id < MAXPLAYER)
		{
			
			player = reinterpret_cast<Player*>(objects[user_id]);
		}
		else if (user_id == 99999)
		{
			cout << "NEW ACEPT" << endl;
		}
		else
		{
			cout << " ACCEPT ERROR " << endl;
			DebugBreak();
		}

		switch (wsa_over_ex->_iocpop)
		{
		case OP_RECV:
		{
			unsigned char* packet_start = wsa_over_ex->_buf;
			int remain_data = io_byte + player->_prev_size;
			cout << user_id<< "// byte : " << remain_data << endl;
			while (remain_data > 0)
			{
				int packet_size = packet_start[0];
				if (packet_size <= remain_data)
				{
					player->_wsa_recv_over.processpacket(user_id, wsa_over_ex->_buf);
					packet_start += packet_size;
					remain_data -= packet_size;
				}
				else
					break;
			}
			player->_prev_size = remain_data;
			if(remain_data > 0)
			{
				memcpy(wsa_over_ex->_buf, packet_start, remain_data);
			}
			DWORD flags = 0;			
			ZeroMemory(&player->_wsa_recv_over._wsaover, sizeof(player->_wsa_recv_over._wsaover));
			player->_wsa_recv_over._wsabuf.buf = reinterpret_cast<char*>(player->_wsa_recv_over._buf + player->_prev_size);
			player->_wsa_recv_over._wsabuf.len = BUFSIZE - remain_data;
			WSARecv(player->_socket, &player->_wsa_recv_over._wsabuf, 1, NULL, &flags, &player->_wsa_recv_over._wsaover, NULL);
			break;
		}
		case OP_SEND:
		{
			cout << io_byte << endl;
			delete wsa_over_ex;
			break;
		}
		case OP_ACCEPT:
		{
			cout << "ACCEPT!" << endl;
			user_id = player_cnt++;
			CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), h_iocp, user_id, 0);
			Player* n_player = reinterpret_cast<Player*>(objects[user_id]);
			ZeroMemory(&n_player->_wsa_recv_over, sizeof(n_player->_wsa_recv_over));
			n_player->_socket = c_socket;
			n_player->_wsa_recv_over._iocpop = OP_RECV;
			n_player->_prev_size = 0;
			n_player->_id = user_id;
			n_player->_wsa_recv_over._wsabuf.buf = reinterpret_cast<char*>(n_player->_wsa_recv_over._buf);
			n_player->_wsa_recv_over._wsabuf.len = BUFSIZE;
			n_player->isconnect = true;
			DWORD flags = 0;
			WSARecv(c_socket, &n_player->_wsa_recv_over._wsabuf, 1, NULL, &flags, &n_player->_wsa_recv_over._wsaover, NULL);

			c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
			ZeroMemory(&accept_over, sizeof(accept_over));
			accept_over.set_accept_over();

			AcceptEx(s_socket, c_socket, accept_over.getbuf(), NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accept_over.getWsaOver());

			break;
		}
		default:
			break;
		}

	}
	
	return 0;
}
