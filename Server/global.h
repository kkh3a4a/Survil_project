#pragma once
#include <cmath>
#include<map>
#include<Windows.h>
#include<random>
#include<vector>
#include<chrono>
#include<iostream>
#define MAXPLAYER 1
#define MAXCITIZEN 200
#define FIRSTSPAWN 10

std::uniform_int_distribution <int>uid{ 0 + 100 * 100, 1600 * 100 - 100 * 100 };
std::uniform_int_distribution <int>resource_uid{ -100, 100 };
typedef struct three_float {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
}TF;

typedef struct FActor_location_rotation {
	TCHAR name[30];
	TF location;
	TF rotation;
}FActor;

typedef struct FCitizen_sole {
	TCHAR name[30];
	TF location;
	TF rotation;
	int resources[5];
	int HP;
	int job;				/////////////// 0 : 무직, 1 : 자원 채취
	int isJob;		
	TF Job_location;		
}FCitizen_sole;

typedef struct location_rotation
{
	TF location;
	TF rotation;
}location_rotation;


typedef struct Citizen_moving
{
	int team;
	int citizen_number;
	TF location;
	TF rotation;
	int citizen_job;		/////////////// 0 : 무직, 1 : 자원 채취
}Citizen_moving;

typedef struct keyboard_input {
	bool w;
	bool a;
	bool s;
	bool d;
}keyboard_input;

typedef struct players_profile {
	int port;
	FActor* player_info;
	TF curr_location;
	TF city_location;
	keyboard_input my_keyinput;
	std::vector<FCitizen_sole*> player_citizen;
	std::vector<Citizen_moving*> player_citizen_arrival_location;
	std::chrono::steady_clock::time_point resource_clcok;
	int resources[5] = {};
}PP;

typedef struct resource_actor
{
	int type;		///////////////0 : 석유,		1 : 물,		2 : 철,		3 : 식량,	4 : 나무
	int count;
	TF location;
	int CitizenCount = 0;
}resource_actor;

typedef struct UI_resource_Input {
public:
	int ResourceNum;
	bool CitizenCountAdd;
	bool CitizenCountSub;

}UI_resource_Input;

typedef struct UI_Input {
public:
	UI_resource_Input resource_input;
}UI_Input;


typedef struct FirstSendServer{
public:
	TF SunAngle = { 0,0,0 };
	FActor player_info;
	FCitizen_sole player_citizen[MAXPLAYER][MAXCITIZEN];
	resource_actor resources[MAXPLAYER * 10];
};

void FActor_TF_define(TF& a, TF& b)
{
	a.x = b.x;
	a.y = b.y;
	a.z = b.z;
}

bool TF_Same(TF a, TF b)
{
	if (a.x == b.x && a.y == b.y)
		return true;
	return false;
}

float location_distance(TF& p1, TF& p2)
{
	float distance;

	// 피타고라스의 정리
	// pow(x,2) x의 2승,  sqrt() 제곱근
	distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

	return distance;
}

void Move_Civil(TF& civil, TF& end_location) {
	float distance;
	distance = location_distance(civil, end_location);

	civil.x += ((end_location.x - civil.x) / distance) * 21;
	civil.y += ((end_location.y - civil.y) / distance) * 21;
	civil.z += ((end_location.z - civil.z) / distance) * 21;
}
int CitizenResourceCount(int resource[5])
{
	int temp = 0;
	for (int i = 0; i < 5; ++i)
	{
		temp += resource[i];
	}

	return temp;
}

void player_random_location(std::map<int, players_profile*>& players_list, std::map <int, Citizen_moving*>& citizen_Move)
{
	std::default_random_engine dre2;
	dre2.seed(std::chrono::system_clock::now().time_since_epoch().count());
	for (auto& a : players_list)
	{
	retry:
		a.second->player_info = new FActor;
		_stprintf_s(a.second->player_info->name, _countof(a.second->player_info->name), _T("%d"), a.first);
		a.second->player_info->location.x = uid(dre2);
		a.second->player_info->location.y = uid(dre2);
		for (auto& b : players_list)
		{
			if (a.first != b.first)
			{
				if (location_distance(a.second->player_info->location, b.second->player_info->location) < 7000)
				{
					goto retry;
				}
			}
		}

		a.second->curr_location.x = a.second->player_info->location.x;
		a.second->curr_location.y = a.second->player_info->location.y;
		a.second->curr_location.z = a.second->player_info->location.z;
		a.second->my_keyinput.w = false;
		a.second->my_keyinput.s = false;
		a.second->my_keyinput.a = false;
		a.second->my_keyinput.d = false;
		a.second->player_citizen.resize(MAXCITIZEN);
		a.second->player_citizen_arrival_location.resize(MAXCITIZEN);
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				FCitizen_sole* temp = new FCitizen_sole;
				Citizen_moving* temp_citizen_move = new Citizen_moving;
				temp->location.x = a.second->player_info->location.x + i * 500 + 2000;
				temp->location.y = a.second->player_info->location.y + j * 500 - 500;
				temp_citizen_move->location.x = temp->location.x;
				temp_citizen_move->location.y = temp->location.y;
				temp->resources[0] = 0;
				temp->resources[1] = 0;
				temp->resources[2] = 0;
				temp->resources[3] = 0;
				temp->resources[4] = 0;
				temp->job = 0;
				a.second->player_citizen[i * 5 +j] = temp;
				a.second->player_citizen_arrival_location[i * 5 + j] = temp_citizen_move;
				
				std::cout << a.second->player_citizen[i * 5 + j]->location.x << ", " << a.second->player_citizen[i * 5 + j]->location.y << std::endl;
			}
		}
	}

	//return true;
}

bool create_resource_location(std::map<int, players_profile*>& players_list, std::map<int, resource_actor*>& resource_create_landscape) {
	
	int cnt = 0;
	std::default_random_engine dre2;
	for (auto& a : players_list)
	{

		for (int i = 0; i < 10; ++i)
		{
			resource_actor* temp = new resource_actor;
			temp->count = 20;
			temp->type = i % 5;
			do
			{
				temp->location.x = a.second->player_info->location.x + resource_uid(dre2) * 50;
				temp->location.y = a.second->player_info->location.y + resource_uid(dre2) * 50;
			} while (location_distance(a.second->player_info->location, temp->location) < 2000);
			resource_create_landscape.insert({ cnt * 10+ i,temp });

		}
		cnt++;
	}
	return true;
}

void resource_collect(std::map<int, players_profile*>& players_list, std::map<int, resource_actor*>& resource_create_landscape) {
	
	for (auto& a : players_list)
	{
		int cnt = 0;
		for (auto& citizens : a.second->player_citizen)
		{
			if(citizens != NULL)
			{
				for (auto& resources : resource_create_landscape)
				{
					int resource_count = 0;
					for (int i = 0; i < 5; ++i)
					{
						resource_count += citizens->resources[i];
					}
					if (location_distance(citizens->location, resources.second->location) < 10)
					{
						if (resources.second->count <= 0)
						{
							citizens->job = 0;
						}
						else if (resource_count < 10)
						{
							citizens->resources[resources.second->type]++;
							resources.second->count--;
						}
					}
					if (resource_count >= 10)
					{

						a.second->player_citizen_arrival_location[cnt]->location.x = a.second->player_info->location.x;
						a.second->player_citizen_arrival_location[cnt]->location.y = a.second->player_info->location.y;
					}
					if (resources.second->count == 0 &&
						citizens->Job_location.x == resources.second->location.x &&
						citizens->Job_location.y == resources.second->location.y)
					{
						a.second->player_citizen_arrival_location[cnt]->location.x = a.second->player_info->location.x;
						a.second->player_citizen_arrival_location[cnt]->location.y = a.second->player_info->location.y;
						resources.second->CitizenCount = 0;
						citizens->job = 0;
					}
					for (int i = 0; i < 5; ++i)
					{
						if (citizens->job == 0 && citizens->resources[i] > 0)
						{
							a.second->player_citizen_arrival_location[cnt]->location.x = a.second->player_info->location.x;
							a.second->player_citizen_arrival_location[cnt]->location.y = a.second->player_info->location.y;
						}
					}
					if (location_distance(citizens->location, a.second->player_info->location) < 1550)
					{
						for (int i = 0; i < 5; ++i)
						{
							a.second->resources[i] += citizens->resources[i];
							citizens->resources[i] = 0;
						}
						if (citizens->job != 0)
						{
							a.second->player_citizen_arrival_location[cnt]->location.x = citizens->Job_location.x;
							a.second->player_citizen_arrival_location[cnt]->location.y = citizens->Job_location.y;
						}
						else if (citizens->job == 0)
						{
							a.second->player_citizen_arrival_location[cnt]->location.x = citizens->location.x;
							a.second->player_citizen_arrival_location[cnt]->location.y = citizens->location.y;
						}
					}
				}
				cnt++;
			}
		}
	}
}

void camera_movement(std::map<int, players_profile*>& players_list)
{
	for (auto& a : players_list)
	{
		if (a.second->my_keyinput.w)
		{
			a.second->curr_location.y -= 100;
		}
		if (a.second->my_keyinput.s)
		{
			a.second->curr_location.y += 100;
		}
		if (a.second->my_keyinput.a)
		{
			a.second->curr_location.x -= 100;
		}
		if (a.second->my_keyinput.d)
		{
			a.second->curr_location.x += 100;
		}
	}
}

void mouse_input_checking(Citizen_moving& temp_citizen_moving, std::map<int, players_profile*>& players_list, int port) {
	if (temp_citizen_moving.team != -1) {
		players_list[port]->player_citizen_arrival_location[temp_citizen_moving.citizen_number]->team = temp_citizen_moving.team;
		players_list[port]->player_citizen_arrival_location[temp_citizen_moving.citizen_number]->location.x = temp_citizen_moving.location.x;
		players_list[port]->player_citizen_arrival_location[temp_citizen_moving.citizen_number]->location.y = temp_citizen_moving.location.y;
		if (temp_citizen_moving.citizen_job != 0) {
			players_list[port]->player_citizen[temp_citizen_moving.citizen_number]->job = temp_citizen_moving.citizen_job;
			players_list[port]->player_citizen[temp_citizen_moving.citizen_number]->Job_location.x = temp_citizen_moving.location.x;
			players_list[port]->player_citizen[temp_citizen_moving.citizen_number]->Job_location.y = temp_citizen_moving.location.y;
		}
	}
}

void Citizen_Work_Add(std::map<int, players_profile*>& players_list, std::map<int, resource_actor*>& resource_create_landscape, int port, int i)
{
	int j = 0;
	float proximate = 210'000'000;
	float location = -210'000'000;
	FCitizen_sole* findCitizen = NULL;
	int count = 0;
	for (auto& a : players_list[port]->player_citizen)
	{
		if (CitizenResourceCount(a->resources) == 0)
		{
			if (a->job == 0)
			{
				location = location_distance(a->location, resource_create_landscape[i]->location);
				if (location < proximate)
				{
					proximate = location;
					findCitizen = a;
					count = j;
				}
			}
		}
		j++;
	}
	if (findCitizen != NULL)
	{

		FActor_TF_define(findCitizen->Job_location, resource_create_landscape[i]->location);
		FActor_TF_define(players_list[port]->player_citizen_arrival_location[count]->location, resource_create_landscape[i]->location);
		resource_create_landscape[i]->CitizenCount++;
		findCitizen->job = 1;
		return;
	}

	proximate = 210'000'000;
	location = -210'000'000;
	j = 0;
	for (auto& a : players_list[port]->player_citizen)
	{
		if (a->job == 0)
		{
			FActor_TF_define(a->Job_location, resource_create_landscape[i]->location);
			FActor_TF_define(players_list[port]->player_citizen_arrival_location[j]->location, resource_create_landscape[i]->location);
			resource_create_landscape[i]->CitizenCount++;
			a->job = 1;
			return;
			location = location_distance(a->location, resource_create_landscape[i]->location);
			if (location < proximate)
			{
				proximate = location;
				findCitizen = a;
				count = j;
			}
		}
		j++;
	}

	if (findCitizen != NULL)
	{
		FActor_TF_define(findCitizen->Job_location, resource_create_landscape[i]->location);
		FActor_TF_define(players_list[port]->player_citizen_arrival_location[count]->location, resource_create_landscape[i]->location);
		resource_create_landscape[i]->CitizenCount++;
		findCitizen->job = 1;
		return;
	}
}

void Citizen_Work_Sub(std::map<int, players_profile*>& players_list, std::map<int, resource_actor*>& resource_create_landscape, int port, int i)
{
	int j = 0;
	for (auto& a : players_list[port]->player_citizen)
	{
		if (a->job != 0)
		{
			if (TF_Same(a->Job_location, resource_create_landscape[i]->location))
			{
				a->job = 0;
				FActor_TF_define(players_list[port]->player_citizen_arrival_location[j]->location, players_list[port]->player_info->location);
				resource_create_landscape[i]->CitizenCount--;
				return;
			}
		}
		j++;
	}

}

void FirstInit(FirstSendServer& first_send_server, std::map<int, players_profile*>& players_list, std::map<int, resource_actor*>& resource_create_landscape, int port) {
	
	memcpy(&first_send_server.player_info, players_list[port]->player_info, sizeof(FActor));
	players_list[port]->player_info = &first_send_server.player_info;
	for (int i = 0; i < FIRSTSPAWN; ++i)
	{
		memcpy(&first_send_server.player_citizen[0][i], players_list[port]->player_citizen[i], sizeof(FCitizen_sole));
		players_list[port]->player_citizen[i] = &first_send_server.player_citizen[0][i];
	}
	for (int i = 0; i < MAXPLAYER * 10; ++i)
	{
		memcpy(&first_send_server.resources[i], resource_create_landscape[i], sizeof(resource_actor));
		resource_create_landscape[i] = &first_send_server.resources[i];
	}
}