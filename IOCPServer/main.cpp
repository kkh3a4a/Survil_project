//#include <iostream>
//#include <fstream>
//#include<vector>
#include<mutex>
#include<string>
//#include<map>
//#include <chrono>
#include<shared_mutex>
//#include<algorithm>

#include"Network.h"
#include"terrain.cu"

#pragma comment (lib,"WS2_32.lib")
#pragma comment (lib,"MSWSock.lib")

using namespace std;
using namespace chrono;

uniform_int_distribution <int>map_uid{ 100, one_side_number - 100 };
//함수정의 
DWORD WINAPI ProcessClient(LPVOID arg);
DWORD WINAPI ingame_thread(LPVOID arg);

bool game_start = false;
int len = 0;

std::mutex player_cnt_lock;
shared_mutex player_list_lock;
float sun_angle;
volatile int player_cnt = 0;
volatile int room_player_cnt = 0;

Terrain* terrain = new Terrain();
char** total_terrain = terrain->get_map();
char** shadow_map = terrain->get_shadow_map();
unsigned char** temperature_map = terrain->get_temperature_map();
bool is_terrain_changed = false;

DWORD WINAPI matching_thread(LPVOID arg)
{
	HANDLE hThread;
	bool isoncecreate = 1;
	while (1)
	{
		if (room_player_cnt == ROOMPLAYER && isoncecreate)
		{
			hThread = CreateThread(NULL, 0, ingame_thread, 0, 0, NULL);
			room_player_cnt = 0;
			isoncecreate = 0;
		}
	}
	return 0;
}

DWORD WINAPI terrain_change(LPVOID arg)
{
	//terrain->log_on();
	int i{};
	auto terrain_start = std::chrono::system_clock::now();
	while (1){
		auto terrain_end = std::chrono::system_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(terrain_end - terrain_start).count() > 1000 && !is_terrain_changed)
		{
			clock_t start = clock();
			terrain_start = std::chrono::system_clock::now();
			cout << endl << i << "번째" << endl;

			terrain->wind_blow({ 1, 0 }, 1);
			terrain->add_object_height();
			terrain->make_shadow_map(sun_angle);
			terrain->make_tempertature_map(sun_angle);
			CC retval = terrain->get_highest_lowest(temperature_map);

			cout << "Temperature Highest: " << (float)retval.x / 4 << ", Lowest" << (float)retval.y / 4 << endl;

			//terrain->show_array(total_terrain, 320);
			//terrain->show_array(shadow_map, 320);
			//terrain->show_array(temperature_map, 320);

			if (i % 100 == 0 && i != 0) {
				terrain->save_terrain();
				cout << "SAVED!!!" << endl;
			}
			i++;
			clock_t end = clock();
			cout << "걸린시간: " << (double)(end - start) / CLOCKS_PER_SEC << endl;
			is_terrain_changed = true;
		}
		else
			Sleep(10);
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
	cout << "ingame thread " << ROOMPLAYER <<"명 짜리 생성" << endl;
	auto Player_Move_Timer_End = std::chrono::system_clock::now();
	auto Citizen_Move_Timer_End = std::chrono::system_clock::now();
	auto Resource_Collect_Timer_End = std::chrono::system_clock::now();
	auto TerrainSend_Timer_End = std::chrono::system_clock::now();
	object_z = terrain->get_map();
	bool IsOnceWork = true;
	char** player_terrain = terrain->get_player_sight_map();
	char** player_temperature = terrain->get_player_temperature_map();
	int citizen_eat = 0;

	for (int i = 0; i < MAXPLAYER; ++i)
	{
		Player* player = reinterpret_cast<Player*>(objects[i]);
		TF player_city{ player->_x, player->_y };
		terrain->set_city_location(TF{player->_x, player->_y}, i);
	}

	while (1)
	{
		auto Timer_Start = std::chrono::system_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(Timer_Start - Player_Move_Timer_End).count() > 10)
		{
			//sunangle...
			auto cycle_time = duration_cast<milliseconds>(Timer_Start - Player_Move_Timer_End).count();
			Player_Move_Timer_End = std::chrono::system_clock::now();
			//rotate sunangle
			//태양각도 1초에 2도 돌아서 180초에 360도 (3분에 한바퀴)
			
			sun_angle += 2.f * cycle_time / 1000.f;
			//sun_angle = 45.f;
			if (sun_angle >= 360.f) 
			{
				sun_angle -= 360.f;
				IsNight = false;
				IsOnceWork = true;
				citizen_eat = 0;
				for (int i = CITIZENSTART; i < MAXCITIZEN + CITIZENSTART; ++i)
				{
					Citizen* citizen = reinterpret_cast<Citizen*>(objects[i]);
					if (citizen->_job == 1)
					{
						citizen->_arrival_x = citizen->_job_x;
						citizen->_arrival_y = citizen->_job_y;
					}
				}
			}
			else if (sun_angle >= 180.f)
			{
				IsNight = true;
			}
			for (int i = 0; i < MAXPLAYER; ++i)
			{
				Player* player = reinterpret_cast<Player*>(objects[i]);
				player->send_sunangle(sun_angle);		//태양 각도 전송

				if (player->is_connected && (player->w || player->a || player->s || player->d))
				{
					char** player_sight_terrain_line = terrain->copy_for_player_map_line((int)(player->_x + player->_currentX) / 100, (int)(player->_y + player->_currentY) / 100);
					char** player_sight_temperature_line = terrain->copy_for_player_temperature_line((int)(player->_x + player->_currentX) / 100, (int)(player->_y + player->_currentY) / 100);

					player->key_input(player_sight_terrain_line, player_sight_temperature_line);
					player->send_sunangle(sun_angle);
				}
			}
			// 건물 온도 업데이트
			/*for (int i = BUILDINGSTART; i < BUILDINGSTART + MAXBUILDING; ++i) {
				objects[i]
			}*/

			if (sun_angle - citizen_eat > 10)
			{
				citizen_eat = sun_angle;
				for (int player_id = 0; player_id < MAXPLAYER; ++player_id)
				{
					Player* player = reinterpret_cast<Player*>(objects[player_id]);
					int citizencount = player->playercitizencount();
					for (int citizen_id = CITIZENSTART + player_id * PLAYERCITIZENCOUNT; citizen_id < CITIZENSTART + (player_id + 1) * PLAYERCITIZENCOUNT; ++citizen_id)
					{
						Citizen* citizen = reinterpret_cast<Citizen*>(objects[citizen_id]);
						if (citizen->_job == -1)
							continue;
						citizen->_satiety -= 1;
						citizen->_thirsty -= 1;

						if (citizen->_satiety == 0)
						{
							if (!citizen->citizen_eat_food())
								citizen->citizen_dead();
						}
						if (citizen->_thirsty == 0)
						{
							if (!citizen->citizen_eat_water())
								citizen->citizen_dead();
						}

						if (player->_resource_amount[3] > citizencount)
						{
							if (citizen->_satiety < 70)
								citizen->citizen_eat_food();
						}
						else
						{
							if (citizen->_satiety < 30)
								citizen->citizen_eat_food();
						}

						if (player->_resource_amount[1] > citizencount)
						{
							if (citizen->_thirsty < 70)
								citizen->citizen_eat_water();
						}
						else
						{
							if (citizen->_thirsty < 30)
								citizen->citizen_eat_water();
						}
					}

					player->send_resource_amount();
				}
			}
		}
		
	
		
		if (std::chrono::duration_cast<std::chrono::milliseconds>(Timer_Start - Citizen_Move_Timer_End).count() > 10)
		{
			Citizen_Move_Timer_End = std::chrono::system_clock::now();
			for (int i = CITIZENSTART; i < MAXCITIZEN + CITIZENSTART; ++i)
			{
				Citizen* citizen = reinterpret_cast<Citizen*>(objects[i]);
				if (citizen->_job == -1 || citizen->_job == 22)
				{
					continue;
				}
				citizen->set_citizen_move();
			}
			for (int i = ARMYSTART; i < ARMYMAX + ARMYSTART; ++i)
			{
				Army* army = reinterpret_cast<Army*>(objects[i]);
				if (army->_a_state != Army::ST_CONPLETE)
				{
					continue;
				}
				army->set_army_move();
			}
		}
		if (std::chrono::duration_cast<std::chrono::milliseconds>(Timer_Start - Resource_Collect_Timer_End).count() > 5000)	//5000
		{	
			Resource_Collect_Timer_End = std::chrono::system_clock::now();
			for (int i = RESOURCESTART; i < RESOURCESTART + MAXRESOURCE; ++i)
			{
				Resource* resource = reinterpret_cast<Resource*>(objects[i]);
				resource->collect_resource();
			}
			for (int i = BUILDINGSTART; i < MAXBUILDING + BUILDINGSTART; ++i)
			{
				Building* building = reinterpret_cast<Building*>(objects[i]);
				building->WorkBuilding();
			}
			for (int i = 0; i < MAXPLAYER; ++i)
			{
				Player* player = reinterpret_cast<Player*>(objects[i]);
				player->send_resource_amount();
			}
		}
		for (int i = 0; i < MAXPLAYER; ++i)
		{
			Player* player = reinterpret_cast<Player*>(objects[i]);
			if (player->_Minimap_terrainsend == 1)
			{
				terrain->copy_for_player_map(II{ (int)(player->_x + player->_currentX) / 100, (int)(player->_y + player->_currentY) / 100 });
				if (player->is_connected)
				{
					while (1)
					{
						//Terrain
						static unsigned char terrain_x{};
						if (terrain_x == SIGHT_X)
						{
							terrain_x = 0;
							break;
						}
						sc_packet_terrainAll packet;
						packet.size = sizeof(sc_packet_terrainAll);
						packet.type = SC_PACKET_TERRAINALL;
						packet.terrain_X = terrain_x;
						memcpy(packet.terrain_Y, player_terrain[terrain_x], SIGHT_Y);

						player->send_packet(&packet);
						terrain_x++;
					}
					//Temperature
					while (1)
					{
						static unsigned char terrain_x{};
						if (terrain_x == SIGHT_X)
						{
							terrain_x = 0;
							break;
						}
						sc_packet_temperature packet;
						packet.terrain_X = terrain_x;
						memcpy(packet.terrain_Y, player_temperature[terrain_x], SIGHT_Y);

						player->send_packet(&packet);
						terrain_x++;
					}
				}
				player->_Minimap_terrainsend = 0;
			}
		}

		if (is_terrain_changed)
		{
			for (int i = 0; i < MAXPLAYER; ++i)
			{
				Player* player = reinterpret_cast<Player*>(objects[i]);
				terrain->copy_for_player_map(II{ (int)(player->_x + player->_currentX) / 100, (int)(player->_y + player->_currentY) / 100});
				if(player->is_connected)
				{
					while(1)
					{
						//Terrain
						static unsigned char terrain_x{};
						if (terrain_x == SIGHT_X)
						{
							terrain_x = 0;
							break;
						}
						sc_packet_terrainAll packet;
						packet.size = sizeof(sc_packet_terrainAll);
						packet.type = SC_PACKET_TERRAINALL;
						packet.terrain_X = terrain_x;
						memcpy(packet.terrain_Y, player_terrain[terrain_x], SIGHT_Y);

						player->send_packet(&packet);
						terrain_x++;
					}
					//Temperature
					while (1)
					{
						static unsigned char terrain_x{};
						if (terrain_x == SIGHT_X)
						{
							terrain_x = 0;
							break;
						}
						sc_packet_temperature packet;
						packet.terrain_X = terrain_x;
						memcpy(packet.terrain_Y, player_temperature[terrain_x], SIGHT_Y);

						player->send_packet(&packet);
						terrain_x++;
					}
				}
			}	
			is_terrain_changed = false;
		}
		if (IsNight)
		{
			if(IsOnceWork)
			{
				for (int citizen_id = CITIZENSTART; citizen_id < MAXCITIZEN + CITIZENSTART; ++citizen_id)
				{
					int player_id = (citizen_id - 5) / 200;
					Player* player = reinterpret_cast<Player*>(objects[player_id]);
					Citizen* citizen = reinterpret_cast<Citizen*>(objects[citizen_id]);
					if (citizen->_job == -1)
						continue;
					
					if (citizen->_house_id == -1)
					{
						bool isHouseStaff = false;
						//만약 빈 HOUSE가 생겼을때 시민을 배치해준다.
						for (int building_id = BUILDINGSTART + player_id * PLAYERBUILDINGCOUNT; building_id < BUILDINGSTART + (player_id + 1) * PLAYERBUILDINGCOUNT; building_id++)
						{
							Building* building = reinterpret_cast<Building*>(objects[building_id]);
							
							if (building->_type == 1 || building->_type == 2 || building->_type == 3)
							{
								for (auto& a : building->_citizens)
								{
									if (a == nullptr)
									{
										citizen->_house_id = building->_id;
										a = citizen;
										citizen->_arrival_x = objects[citizen->_house_id]->_x;
										citizen->_arrival_y = objects[citizen->_house_id]->_y;
										isHouseStaff = true;
										break;
									}
								}
							}
							if (isHouseStaff)
								break;

						}
						if(!isHouseStaff)
						{
							int rocate = 1;
							if (rand() % 2 == 0)
								rocate *= -1;
							citizen->_arrival_x = player->_x + (rand() % 500) * rocate + 500 * rocate;
							if (rand() % 2 == 0)
								rocate *= -1;
							citizen->_arrival_y = player->_y + rand() % 500 * rocate + 500 * rocate;
						}
					}
					else
					{
						citizen->_arrival_x = objects[citizen->_house_id]->_x;
						citizen->_arrival_y = objects[citizen->_house_id]->_y;
					}
				}
				IsOnceWork = false;
			}
		}
		///////////////////////

	}
	return 0;
}

int main(int argc, char* argv[])
{	
	cout << fixed;
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
		Player* player = reinterpret_cast<Player*>(objects[i]);
		
	}
	for (int i = MAXPLAYER; i < MAXCITIZEN + MAXPLAYER; ++i)//5~1004까지는 Citizen
	{
		objects[i] = new Citizen(i);
	}
	for (int i = RESOURCESTART; i < RESOURCESTART + MAXRESOURCE; ++i)//1005 ~ 1054까지는 Resource
	{
		objects[i] = new Resource(i ,i % 5);	//i%5 해서 type을 정해줌 /0 : 석유,	1 : 물,		2 : 철,		3 : 식량,		4 : 나무
	}
	for (int i = BUILDINGSTART; i < BUILDINGSTART + MAXBUILDING; ++i)//1055 ~ 1659까지는 Building	인당 11*11개 = 121개
	{
		objects[i] = new Building(i);
	}

	for (int i = ARMYSTART; i < ARMYSTART + ARMYMAX; ++i)//1055 ~ 1659까지는 Building	인당 11*11개 = 121개
	{
		objects[i] = new Army(i);
	}
	for (int i = EVENTSTART; i < EVENTMAX + EVENTSTART; ++i)
	{
		objects[i] = new GameEvent(i);
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
			if (location_distance(objects[j]->_x, objects[j]->_y, x, y) < 50000)
			{
				goto retry;
			}
		}
		reinterpret_cast<Player*>(objects[i])->set_player_location(x, y, z);
		//reinterpret_cast<Player*>(objects[i])->player_sight_terrain = total_terrain;
		
		for (int line = 0; line != FIRSTCITIZENCREATE / 10; ++line)
		{
			for (int j = 0; j < 10; ++j)
			{
				reinterpret_cast<Citizen*>(objects[MAXPLAYER + i * PLAYERCITIZENCOUNT + j + (line * 10)])->set_citizen_spwan_location(x + 2000 + (line * 500), y + (j * 500) - 2250, z);
			}
		}
		for (int j = 0; j < 10; ++j)
		{
			reinterpret_cast<Resource*>(objects[RESOURCESTART + i * 10 + j])->set_resource_spwan_location(x, y, z);
		}
		for (int e_id = EVENTSTART; e_id < EVENTMAX + EVENTSTART; ++e_id)
			reinterpret_cast<GameEvent*>(objects[e_id])->random_create();
	}

	//다 지정해주고 thread생성
	HANDLE hThread;

	hThread = CreateThread(NULL, 0, terrain_change, 0, 0, NULL);
	hThread = CreateThread(NULL, 0, matching_thread, 0, 0, NULL);
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
				room_player_cnt--;
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
			//cout << user_id<< "// byte : " << remain_data << endl;
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
			delete wsa_over_ex;
			break;
		}
		case OP_ACCEPT:
		{
			cout << "ACCEPT!" << endl;
			user_id = player_cnt++;
			room_player_cnt++;
			CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), h_iocp, user_id, 0);
			Player* n_player = reinterpret_cast<Player*>(objects[user_id]);
			ZeroMemory(&n_player->_wsa_recv_over, sizeof(n_player->_wsa_recv_over));
			n_player->_socket = c_socket;
			n_player->_wsa_recv_over._iocpop = OP_RECV;
			n_player->_prev_size = 0;
			n_player->_id = user_id;
			n_player->_wsa_recv_over._wsabuf.buf = reinterpret_cast<char*>(n_player->_wsa_recv_over._buf);
			n_player->_wsa_recv_over._wsabuf.len = BUFSIZE;
			
			//n_player->player_sight_terrain = terrain->get_player_sight_map();
			DWORD flags = 0;
			WSARecv(c_socket, &n_player->_wsa_recv_over._wsabuf, 1, NULL, &flags, &n_player->_wsa_recv_over._wsaover, NULL);
			n_player->is_connected = true;
			n_player->_state = STATE::ST_INGAME;
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
