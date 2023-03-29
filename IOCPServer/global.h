#pragma once
#include"NewGlobal.h"




//
//
//std::uniform_int_distribution <int>uid{ 0 + 1000, one_side_number - 1000 };
//std::uniform_int_distribution <int>resource_uid{ -100, 100 };
//
//
//
//typedef struct FActor_location_rotation {
//	TCHAR name[30];
//	TF location;
//	TF rotation;
//}FActor;
//
//typedef struct FCitizen_sole {
//	TCHAR name[30];
//	TF location;
//	TF rotation;
//	int resources[5];
//	int HP;
//	int job;				/////////////// 0 : 무직, 1 : 자원 채취
//	int isJob;		
//	TF Job_location;		
//}FCitizen_sole;
//
//typedef struct location_rotation
//{
//	TF location;
//	TF rotation;
//}location_rotation;
//
//typedef struct Citizen_moving
//{
//	int team;
//	int citizen_number;
//	TF location;
//	TF rotation;
//	int citizen_job;		/////////////// 0 : 무직, 1 : 자원 채취
//}Citizen_moving;
//
//typedef struct keyboard_input {
//	int w;
//	int a;
//	int s;
//	int d;
//}keyboard_input;
//
//typedef struct players_profile {
//	int port;
//	Player _player;
//	std::vector<FCitizen_sole*> player_citizen;
//	std::vector<Citizen_moving*> player_citizen_arrival_location;
//	std::chrono::steady_clock::time_point resource_clcok;
//	int *resources[5] = {};
//}PP;
//
//typedef struct resource_actor
//{
//	int type;		///////////////0 : 석유,		1 : 물,		2 : 철,		3 : 식량,	4 : 나무
//	int count;
//	TF location;
//	int CitizenCount = 0;
//}resource_actor;
//
//typedef struct UI_resource_Input {
//public:
//	int ResourceNum;
//	int CitizenCountAdd;
//	int CitizenCountSub;
//
//}UI_resource_Input;
//
//typedef struct UI_Input {
//public:
//	UI_resource_Input resource_input;
//}UI_Input;
//
//typedef struct sc_sendpacket{
//public:
//	float SunAngle = 0;
//	int MyResource[5];
//	TF player_location = { 0,0,0 };
//	TF current_location = { 0,0,0 };
//	FCitizen_sole player_citizen[MAXPLAYER][MAXCITIZEN];
//	resource_actor resources[MAXPLAYER * 10];
//};
//
//typedef struct sc_sendpacket2 {
//public:
//	
//	//char send_sight_temperature[SIGHT_X][SIGHT_Y];
//};
//
//typedef struct cs_sendpacket {
//public:
//	Citizen_moving My_citizen_moving = {};
//	keyboard_input My_keyboard_input = {};
//	UI_Input My_UI_input = {};
//	int connecting = 1;
//};
//
//void FActor_TF_define(TF& a, TF b)
//{
//	a.x = b.x;
//	a.y = b.y;
//	a.z = b.z;
//}
//
//bool TF_Same(TF a, TF b)
//{
//	if (a.x == b.x && a.y == b.y)
//		return true;
//	return false;
//}
//
//float location_distance(TF p1, TF p2)
//{
//	float distance;
//
//	// 피타고라스의 정리
//	// pow(x,2) x의 2승,  sqrt() 제곱근
//	distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
//
//	return distance;
//}
//
//void Move_Civil(TF& civil, TF& end_location) {
//	float distance;
//	distance = location_distance(civil, end_location);
//
//	civil.x += ((end_location.x - civil.x) / distance) * 21;
//	civil.y += ((end_location.y - civil.y) / distance) * 21;
//	civil.z += ((end_location.z - civil.z) / distance) * 21;
//}
//int CitizenResourceCount(int resource[5])
//{
//	int temp = 0;
//	for (int i = 0; i < 5; ++i)
//	{
//		temp += resource[i];
//	}
//
//	return temp;
//}
//
//void player_random_location(std::map<int, players_profile*>& players_list, std::map <int, Citizen_moving*>& citizen_Move)
//{
//	std::default_random_engine dre2;
//	dre2.seed(std::chrono::system_clock::now().time_since_epoch().count());
//	for (auto& a : players_list)
//	{
//		TF player_location;
//	retry:
//		a.second->_player;
//		//_stprintf_s(a.second->player_info->name, _countof(a.second->player_info->name), _T("%d"), a.first);
//		player_location = { (float)uid(dre2) * UNIT, (float)uid(dre2) * UNIT,0 };
//
//
//		for (auto& b : players_list)
//		{
//			if (a.first != b.first)
//			{
//				if (location_distance(player_location, b.second->_player._getPlayerLocation()) < 7000)
//				{
//					goto retry;
//				}
//			}
//		}
//		a.second->_player._setPlayerLocation(player_location);
//		a.second->player_citizen.resize(MAXCITIZEN);
//		a.second->player_citizen_arrival_location.resize(MAXCITIZEN);
//		for (int i = 0; i < 2; ++i)
//		{
//			for (int j = 0; j < 5; ++j)
//			{
//				FCitizen_sole* temp = new FCitizen_sole;
//				Citizen_moving* temp_citizen_move = new Citizen_moving;
//				temp->location.x = a.second->_player.x + i * 500 + 2000;
//				temp->location.y = a.second->_player.y + j * 500 - 500;
//				temp_citizen_move->location.x = temp->location.x;
//				temp_citizen_move->location.y = temp->location.y;
//				temp->resources[0] = 0;
//				temp->resources[1] = 0;
//				temp->resources[2] = 0;
//				temp->resources[3] = 0;
//				temp->resources[4] = 0;
//				temp->job = 0;
//				a.second->player_citizen[i * 5 +j] = temp;
//				a.second->player_citizen_arrival_location[i * 5 + j] = temp_citizen_move;
//				
//				std::cout << a.second->player_citizen[i * 5 + j]->location.x << ", " << a.second->player_citizen[i * 5 + j]->location.y << std::endl;
//			}
//		}
//	}
//
//	//return true;
//}
//
//bool create_resource_location(std::map<int, players_profile*>& players_list, std::map<int, resource_actor*>& resource_create_landscape) {
//	
//	int cnt = 0;
//	std::default_random_engine dre2;
//	for (auto& a : players_list)
//	{
//
//		for (int i = 0; i < 10; ++i)
//		{
//			resource_actor* temp = new resource_actor;
//			temp->count = 20;
//			temp->type = i % 5;
//			do
//			{
//				temp->location.x = a.second->_player.x + resource_uid(dre2) * 50;
//				temp->location.y = a.second->_player.y + resource_uid(dre2) * 50;
//			} while (location_distance(a.second->_player._getPlayerLocation(), temp->location) < 2000);
//			resource_create_landscape.insert({ cnt * 10+ i,temp });
//
//		}
//		cnt++;
//	}
//	return true;
//}
//
//void resource_collect(std::map<int, players_profile*>& players_list, std::map<int, resource_actor*>& resource_create_landscape) {
//	
//	for (auto& a : players_list)
//	{
//		int cnt = 0;
//		for (auto& citizens : a.second->player_citizen)
//		{
//			if(citizens != NULL)
//			{
//				for (auto& resources : resource_create_landscape)
//				{
//					int resource_count = 0;
//					for (int i = 0; i < 5; ++i)
//					{
//						resource_count += citizens->resources[i];
//					}
//					if (location_distance(citizens->location, resources.second->location) < 10)
//					{
//						if (resources.second->count <= 0)
//						{
//							citizens->job = 0;
//						}
//						else if (resource_count < 10)
//						{
//							citizens->resources[resources.second->type]++;
//							resources.second->count--;
//						}
//					}
//					if (resource_count >= 10)
//					{
//
//						a.second->player_citizen_arrival_location[cnt]->location.x = a.second->_player._getPlayerLocation().x;
//						a.second->player_citizen_arrival_location[cnt]->location.y = a.second->_player._getPlayerLocation().y;
//					}
//					if (resources.second->count == 0 &&
//						citizens->Job_location.x == resources.second->location.x &&
//						citizens->Job_location.y == resources.second->location.y)
//					{
//						a.second->player_citizen_arrival_location[cnt]->location.x = a.second->_player._getPlayerLocation().x;
//						a.second->player_citizen_arrival_location[cnt]->location.y = a.second->_player._getPlayerLocation().y;
//						resources.second->CitizenCount = 0;
//						citizens->job = 0;
//					}
//					for (int i = 0; i < 5; ++i)
//					{
//						if (citizens->job == 0 && citizens->resources[i] > 0)
//						{
//							a.second->player_citizen_arrival_location[cnt]->location.x = a.second->_player._getPlayerLocation().x;
//							a.second->player_citizen_arrival_location[cnt]->location.y = a.second->_player._getPlayerLocation().y;
//						}
//					}
//					if (location_distance(citizens->location, a.second->_player._getPlayerLocation()) < 1550)
//					{
//						for (int i = 0; i < 5; ++i)
//						{
//							*a.second->resources[i] += citizens->resources[i];
//							citizens->resources[i] = 0;
//						}
//						if (citizens->job != 0)
//						{
//							a.second->player_citizen_arrival_location[cnt]->location.x = citizens->Job_location.x;
//							a.second->player_citizen_arrival_location[cnt]->location.y = citizens->Job_location.y;
//						}
//						else if (citizens->job == 0)
//						{
//							a.second->player_citizen_arrival_location[cnt]->location.x = citizens->location.x;
//							a.second->player_citizen_arrival_location[cnt]->location.y = citizens->location.y;
//						}
//					}
//				}
//				cnt++;
//			}
//		}
//	}
//}
//
//void current_movement(std::map<int, players_profile*>& players_list)
//{
//	int movement = 100;
//	for (auto& a : players_list)
//	{
//		
//	}
//}
//
//void mouse_input_checking(Citizen_moving& temp_citizen_moving, std::map<int, players_profile*>& players_list, int port) {
//	if (temp_citizen_moving.team != -1) {
//		players_list[port]->player_citizen_arrival_location[temp_citizen_moving.citizen_number]->team = temp_citizen_moving.team;
//		players_list[port]->player_citizen_arrival_location[temp_citizen_moving.citizen_number]->location.x = temp_citizen_moving.location.x;
//		players_list[port]->player_citizen_arrival_location[temp_citizen_moving.citizen_number]->location.y = temp_citizen_moving.location.y;
//		if (temp_citizen_moving.citizen_job != 0) {
//			players_list[port]->player_citizen[temp_citizen_moving.citizen_number]->job = temp_citizen_moving.citizen_job;
//			players_list[port]->player_citizen[temp_citizen_moving.citizen_number]->Job_location.x = temp_citizen_moving.location.x;
//			players_list[port]->player_citizen[temp_citizen_moving.citizen_number]->Job_location.y = temp_citizen_moving.location.y;
//		}
//	}
//}
//
//void Citizen_Work_Add(std::map<int, players_profile*>& players_list, std::map<int, resource_actor*>& resource_create_landscape, int port, int i)
//{
//	int j = 0;
//	float proximate = 210'000'000;
//	float location = -210'000'000;
//	FCitizen_sole* findCitizen = NULL;
//	int count = 0;
//	for (auto& a : players_list[port]->player_citizen)
//	{
//		if(a != NULL)
//		{
//			if (CitizenResourceCount(a->resources) == 0)
//			{
//				if (a->job == 0)
//				{
//					location = location_distance(a->location, resource_create_landscape[i]->location);
//					if (location < proximate)
//					{
//						proximate = location;
//						findCitizen = a;
//						count = j;
//					}
//				}
//			}
//			j++;
//		}
//	}
//	if (findCitizen != NULL)
//	{
//
//		FActor_TF_define(findCitizen->Job_location, resource_create_landscape[i]->location);
//		FActor_TF_define(players_list[port]->player_citizen_arrival_location[count]->location, resource_create_landscape[i]->location);
//		resource_create_landscape[i]->CitizenCount++;
//		findCitizen->job = 1;
//		return;
//	}
//
//	proximate = 210'000'000;
//	location = -210'000'000;
//	j = 0;
//	for (auto& a : players_list[port]->player_citizen)
//	{
//		if (a != NULL)
//		{
//			if (a->job == 0)
//			{
//				FActor_TF_define(a->Job_location, resource_create_landscape[i]->location);
//				FActor_TF_define(players_list[port]->player_citizen_arrival_location[j]->location, resource_create_landscape[i]->location);
//				resource_create_landscape[i]->CitizenCount++;
//				a->job = 1;
//				return;
//				location = location_distance(a->location, resource_create_landscape[i]->location);
//				if (location < proximate)
//				{
//					proximate = location;
//					findCitizen = a;
//					count = j;
//				}
//			}
//			j++;
//		}
//	}
//
//	if (findCitizen != NULL)
//	{
//		FActor_TF_define(findCitizen->Job_location, resource_create_landscape[i]->location);
//		FActor_TF_define(players_list[port]->player_citizen_arrival_location[count]->location, resource_create_landscape[i]->location);
//		resource_create_landscape[i]->CitizenCount++;
//		findCitizen->job = 1;
//		return;
//	}
//}
//
//void Citizen_Work_Sub(std::map<int, players_profile*>& players_list, std::map<int, resource_actor*>& resource_create_landscape, int port, int i)
//{
//	int j = 0;
//	for (auto& a : players_list[port]->player_citizen)
//	{
//		if (a != NULL)
//		{
//			if (a->job != 0)
//			{
//				if (TF_Same(a->Job_location, resource_create_landscape[i]->location))
//				{
//					a->job = 0;
//					FActor_TF_define(players_list[port]->player_citizen_arrival_location[j]->location, players_list[port]->_player._getPlayerLocation());
//					resource_create_landscape[i]->CitizenCount--;
//					return;
//				}
//			}
//			j++;
//		}
//	}
//
//}
//
//void FirstInit(sc_sendpacket& sc_send, std::map<int, players_profile*>& players_list, int port) 
//{
//
//
//
//
//	for(int i=0; i<5 ;++i)
//	{
//		players_list[port]->resources[i] = new int;
//		*players_list[port]->resources[i] = 0;
//		memcpy(&sc_send.MyResource[i], players_list[port]->resources[i], sizeof(int));
//		players_list[port]->resources[i] = &sc_send.MyResource[i];
//	}	
//
//}
