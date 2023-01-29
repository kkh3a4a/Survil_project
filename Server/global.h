#pragma once
#include <cmath>
#include<map>
#include<Windows.h>
#include<random>
#include<vector>
#include<chrono>
#include<iostream>
#define MAXPLAYER 1

std::uniform_int_distribution <int>uid{ -5000, 5000 };

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
}Citizen_moving;

typedef struct players_profile {
	int port;
	FActor player_info;
	std::vector<FActor*> player_citizen;
	std::vector<Citizen_moving*> player_citizen_arrival_location;
}PP;

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

	civil.x += ((end_location.x - civil.x) / distance) * 20;
	civil.y += ((end_location.y - civil.y) / distance) * 20;
	civil.z += ((end_location.z - civil.z) / distance) * 20;
}

bool player_random_location(std::map<int, players_profile*>& players_list, std::map <int, Citizen_moving*>& citizen_Move)
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
				if (location_distance(a.second->player_info.location, b.second->player_info.location) < 5000)
				{
					goto retry;
				}
			}
		}
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				FActor* temp = new FActor;
				Citizen_moving* temp_citizen_move = new Citizen_moving;
				temp->location.x = a.second->player_info.location.x + i * 500 + 2000;
				temp->location.y = a.second->player_info.location.y + j * 500 - 500;
				temp_citizen_move->location.x = temp->location.x;
				temp_citizen_move->location.y = temp->location.y;
				a.second->player_citizen.emplace_back(temp);
				a.second->player_citizen_arrival_location.emplace_back(temp_citizen_move);


				std::cout << a.second->player_citizen[i * 5 + j]->location.x << ", " << a.second->player_citizen[i * 5 + j]->location.y << std::endl;
			}
		}
	}


	return true;
}