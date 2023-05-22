#pragma once
#include "Player.h"



class Citizen : public Object
{
public:
	Citizen(int id);
	~Citizen();

	char _job;	//	-1 : 생성안됨, 및 사망  // 0 : 생성 및 무직  // 1 : 자원 채취 // 2 : 건물 짓기 // 11 : HunterHouse	// 21 : armycampmove	22 : army
	float _job_x, _job_y, _job_z;
	int _Job_id;
	int _house_id;	// -1 : 집없음, 나머지 : 집 object id
	char _hp;
	char _citizenstate;		// 0 : idle, 1 : wark, 2 : sleep
	char _satiety;
	char _thirsty;

	float _arrival_x, _arrival_y, _arrival_z;
	int _playerID;
	void set_citizen_spwan_location(float x, float y, float z);
	void set_citizen_arrival_location(float ax, float ay, float az);
	void set_citizen_move();
	void citizen_dead();
	bool citizen_eat_food();
	bool citizen_eat_water();
};

