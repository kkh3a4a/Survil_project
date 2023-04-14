#pragma once
#include "Player.h"



class Citizen : public Object
{
public:
	Citizen(int id);
	~Citizen();

	char _Job;	//	-1 : 생성안됨, 및 사망  // 0 : 생성 및 무직  // 1 : 자원 채취 // 2 : ....
	float _job_x, _job_y, _job_z;
	int _HouseId;	// -1 : 집없음, 나머지 : 집 object id
	char _HP;

	char _Satiety;
	char _thirst;

	float _arrival_x, _arrival_y, _arrival_z;
	int _playerID;
	void set_citizen_spwan_location(float x, float y, float z);
	void set_citizen_arrival_location(float ax, float ay, float az);
	void set_citizen_move();
	void citizen_dead();
	bool citizen_eat_food();
	bool citizen_eat_water();
};

