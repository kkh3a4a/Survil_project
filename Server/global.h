#pragma once
#include <cmath>
#include<map>
#include<Windows.h>
#include<random>
#include<vector>
#include<chrono>
#include<iostream>
#define MAXPLAYER 2

std::uniform_int_distribution <int>uid{ -20000, 20000 };
std::uniform_int_distribution <int>resource_uid{ -1001, 1001 };
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
	int resource_type;
	int resource_count;
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
	FActor player_info;
	TF camera_location;
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
}resource_actor;




void FActor_TF_define(TF& a, TF& b)
{
	a.x = b.x;
	a.y = b.y;
	a.z = b.z;
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

void player_random_location(std::map<int, players_profile*>& players_list, std::map <int, Citizen_moving*>& citizen_Move)
{
	std::default_random_engine dre2;
	dre2.seed(std::chrono::system_clock::now().time_since_epoch().count());
	for (auto& a : players_list)
	{
	retry:
		_stprintf_s(a.second->player_info.name, _countof(a.second->player_info.name), _T("%d"), a.first);
		a.second->player_info.location.x = uid(dre2);
		a.second->player_info.location.y = uid(dre2);
		for (auto& b : players_list)
		{
			if (a.first != b.first)
			{
				if (location_distance(a.second->player_info.location, b.second->player_info.location) < 7000)
				{
					goto retry;
				}
			}
		}

		a.second->camera_location.x = a.second->player_info.location.x;
		a.second->camera_location.y = a.second->player_info.location.y + 5000;
		a.second->camera_location.z = a.second->player_info.location.z + 10000;
		a.second->my_keyinput.w = false;
		a.second->my_keyinput.s = false;
		a.second->my_keyinput.a = false;
		a.second->my_keyinput.d = false;
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				FCitizen_sole* temp = new FCitizen_sole;
				Citizen_moving* temp_citizen_move = new Citizen_moving;
				temp->location.x = a.second->player_info.location.x + i * 500 + 2000;
				temp->location.y = a.second->player_info.location.y + j * 500 - 500;
				temp_citizen_move->location.x = temp->location.x;
				temp_citizen_move->location.y = temp->location.y;
				temp->resource_count = 0;
				temp->resource_type = -1;
				temp->job = 0;
				a.second->player_citizen.emplace_back(temp);
				a.second->player_citizen_arrival_location.emplace_back(temp_citizen_move);

				std::cout << a.second->player_citizen[i * 5 + j]->location.x << ", " << a.second->player_citizen[i * 5 + j]->location.y << std::endl;
			}
		}
	}


	//return true;
}


bool create_map_location(std::map<int, players_profile*>& players_list, std::map<int, resource_actor*>& resource_create_landscape) {
	
	int cnt = 0;
	std::default_random_engine dre2;
	for (auto& a : players_list)
	{

		for (int i = 0; i < 10; ++i)
		{
			resource_actor* temp = new resource_actor;
			temp->count = 200;
			temp->type = i;
			temp->location.x = a.second->player_info.location.x + resource_uid(dre2)*5;
			temp->location.y = a.second->player_info.location.y + resource_uid(dre2)*5;
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
			for (auto& resources : resource_create_landscape)
			{
				
				if (location_distance(citizens->location, resources.second->location) < 10)
				{
					if (citizens->resource_count < 10)
					{
						citizens->resource_type = resources.second->type;
						citizens->resource_count++;
						resources.second->count--;
						
					}
				}
				if (citizens->resource_count >= 10)
				{
					
					a.second->player_citizen_arrival_location[cnt]->location.x = a.second->player_info.location.x;
					a.second->player_citizen_arrival_location[cnt]->location.y = a.second->player_info.location.y;
				}
				if (location_distance(citizens->location, a.second->player_info.location) < 1550)
				{
					if(citizens->resource_type != -1)
					{
						a.second->resources[citizens->resource_type] += citizens->resource_count;
						citizens->resource_count = 0;
						citizens->resource_type = -1;
					}
					if (citizens->job != 0)
					{
						a.second->player_citizen_arrival_location[cnt]->location.x = citizens->Job_location.x;
						a.second->player_citizen_arrival_location[cnt]->location.y = citizens->Job_location.y;
					}
				}
			}
			cnt++;
		}


	}
}

void camera_movement(std::map<int, players_profile*>& players_list)
{
	for (auto& a : players_list)
	{
		if (a.second->my_keyinput.w)
		{
			a.second->camera_location.y -= 300;
		}
		if (a.second->my_keyinput.s)
		{
			a.second->camera_location.y += 300;
		}
		if (a.second->my_keyinput.a)
		{
			a.second->camera_location.x -= 300;
		}
		if (a.second->my_keyinput.d)
		{
			a.second->camera_location.x += 300;
		}


	}
}