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
#define SIGHT_X 240
#define SIGHT_Y 100
#define UNIT 100

typedef struct two_int {
	int x;
	int y;
} II;

const int one_side_number = 32000;
II player_sight_size{ SIGHT_X, SIGHT_Y };
const int random_array_size = 90000000;
const int city_size = 100;

std::uniform_int_distribution <int>uid{ 0 + 1000, one_side_number - 1000 };
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
	II location;
	int rotation;
	char resources[5];
	char HP;
	char job;				/////////////// 0 : 무직, 1 : 자원 채취
	char isJob;
	II Job_location;		
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
	FActor* player_info = new FActor;
	TF* curr_location = new TF;
	TF city_location;
	keyboard_input* my_keyinput = new keyboard_input;
	std::vector<FCitizen_sole*> player_citizen;
	std::vector<Citizen_moving*> player_citizen_arrival_location;
	std::chrono::steady_clock::time_point resource_clcok;
	int *resources[5] = {};
}PP;

typedef struct resource_actor
{
	int type;		///////////////0 : 석유,		1 : 물,		2 : 철,		3 : 식량,	4 : 나무
	int count;
	II location;
	int CitizenCount = 0;
}resource_actor;

typedef struct UI_resource_Input {
public:
	int ResourceNum;
	int CitizenCountAdd;
	int CitizenCountSub;

}UI_resource_Input;

typedef struct UI_Input {
public:
	UI_resource_Input resource_input;
}UI_Input;

typedef struct ServerStruct1{
public:
	float SunAngle = 0;
	FActor player_info;
	int MyResource[5];
	TF currlocation = { 0,0,0 };
	//char send_sight_temperature[SIGHT_X][SIGHT_Y];
};

typedef struct ServerStruct2 {
public:
	FCitizen_sole player_citizen[MAXPLAYER][MAXCITIZEN];
	resource_actor resources[MAXPLAYER * 10];
	//char send_sight_temperature[SIGHT_X][SIGHT_Y];
};

typedef struct ClientStruct1 {
public:
	Citizen_moving My_citizen_moving = {};
	keyboard_input My_keyboard_input = {};
	UI_Input My_UI_input = {};
	int connecting = 1;
};

void FActor_TF_define(TF& a, TF& b)
{
	a.x = b.x;
	a.y = b.y;
	a.z = b.z;
}

void FActor_TF_define(II& a, TF& b)
{
	a.x = b.x;
	a.y = b.y;
}
void FActor_TF_define(TF& a, II& b)
{
	a.x = b.x;
	a.y = b.y;
}
void FActor_TF_define(II& a, II& b)
{
	a.x = b.x;
	a.y = b.y;
}

bool TF_Same(TF a, TF b)
{
	if (a.x == b.x && a.y == b.y)
		return true;
	return false;
}
bool TF_Same(II a, TF b)
{
	if (a.x == b.x && a.y == b.y)
		return true;
	return false;
}
bool TF_Same(II a, II b)
{
	if (a.x == b.x && a.y == b.y)
		return true;
	return false;
}

float location_distance(II& p1, TF& p2)
{
	float distance;

	// 피타고라스의 정리
	// pow(x,2) x의 2승,  sqrt() 제곱근
	distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

	return distance;
}
float location_distance(TF& p1, II& p2)
{
	float distance;

	// 피타고라스의 정리
	// pow(x,2) x의 2승,  sqrt() 제곱근
	distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

	return distance;
}

float location_distance(II& p1, II& p2)
{
	float distance;

	// 피타고라스의 정리
	// pow(x,2) x의 2승,  sqrt() 제곱근
	distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

	return distance;
}

float location_distance(TF& p1, TF& p2)
{
	float distance;

	// 피타고라스의 정리
	// pow(x,2) x의 2승,  sqrt() 제곱근
	distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

	return distance;
}
void Move_Civil(II& civil, TF& end_location) {
	float distance;
	distance = location_distance(civil, end_location);

	civil.x += ((end_location.x - civil.x) / distance) * 21;
	civil.y += ((end_location.y - civil.y) / distance) * 21;
	//civil.z += ((end_location.z - civil.z) / distance) * 21;
}


void Move_Civil(TF& civil, TF& end_location) {
	float distance;
	distance = location_distance(civil, end_location);

	civil.x += ((end_location.x - civil.x) / distance) * 21;
	civil.y += ((end_location.y - civil.y) / distance) * 21;
	//civil.z += ((end_location.z - civil.z) / distance) * 21;
}
int CitizenResourceCount(char resource[5])
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
		//_stprintf_s(a.second->player_info->name, _countof(a.second->player_info->name), _T("%d"), a.first);
		a.second->player_info->location.x = uid(dre2) * UNIT;
		a.second->player_info->location.y = uid(dre2) * UNIT;
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
		a.second->curr_location = new TF;
		a.second->curr_location->x = a.second->player_info->location.x;
		a.second->curr_location->y = a.second->player_info->location.y;
		a.second->curr_location->z = a.second->player_info->location.z;
		a.second->my_keyinput->w = false;
		a.second->my_keyinput->s = false;
		a.second->my_keyinput->a = false;
		a.second->my_keyinput->d = false;
		std::cout << a.second->player_info->location.x << std::endl;
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
				std::cout << a.second->player_info->location.x << std::endl;
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
							*a.second->resources[i] += citizens->resources[i];
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
	int movement = 100;
	for (auto& a : players_list)
	{
		if (a.second->my_keyinput->w && a.second->curr_location->y - movement > SIGHT_Y * UNIT)
		{
			a.second->curr_location->y -= movement;
		}
		if (a.second->my_keyinput->s && a.second->curr_location->y + movement < (one_side_number - SIGHT_Y) * UNIT)
		{
			a.second->curr_location->y += movement;
		}
		if (a.second->my_keyinput->a && a.second->curr_location->x - movement > SIGHT_X * UNIT)
		{
			a.second->curr_location->x -= movement;
		}
		if (a.second->my_keyinput->d && a.second->curr_location->x + movement < (one_side_number - SIGHT_X) * UNIT)
		{
			a.second->curr_location->x += movement;
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
		if(a != NULL)
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
		if (a != NULL)
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
		if (a != NULL)
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

}

void FirstInit(ServerStruct1& first_send_server, ClientStruct1& first_send_client, std::map<int, players_profile*>& players_list, char** player_sight_temperature, int port) {
	
	memcpy(&first_send_server.player_info, players_list[port]->player_info, sizeof(FActor));
	players_list[port]->player_info = &first_send_server.player_info;

	for(int i=0; i<5 ;++i)
	{
		players_list[port]->resources[i] = new int;
		*players_list[port]->resources[i] = 0;
		memcpy(&first_send_server.MyResource[i], players_list[port]->resources[i], sizeof(int));
		players_list[port]->resources[i] = &first_send_server.MyResource[i];
	}
	memcpy(&first_send_server.currlocation, players_list[port]->curr_location, sizeof(TF));
	players_list[port]->curr_location = &first_send_server.currlocation;

	memcpy(&first_send_client.My_keyboard_input, players_list[port]->my_keyinput,sizeof(keyboard_input));
	players_list[port]->my_keyinput = &first_send_client.My_keyboard_input;
		

}

void Secondmemcpy(ServerStruct2& second_send_server, std::map<int, players_profile*>& players_list, std::map<int, resource_actor*>& resource_create_landscape, int port)
{
	for (int i = 0; i < FIRSTSPAWN; ++i)
	{
		memcpy(&second_send_server.player_citizen[0][i], players_list[port]->player_citizen[i], sizeof(FCitizen_sole));
	}
	int citizen_cnt = 1;
	for (auto& a : players_list)
	{

		if (a.first != port)
		{
			for (int i = 0; i < FIRSTSPAWN; ++i)
			{
				memcpy(&second_send_server.player_citizen[citizen_cnt][i], players_list[a.first]->player_citizen[i], sizeof(FCitizen_sole));
			}
			citizen_cnt++;
		}

	}
	for (int i = 0; i < MAXPLAYER * 10; ++i)
	{
		memcpy(&second_send_server.resources[i], resource_create_landscape[i], sizeof(resource_actor));
	}
}