#pragma once
#include "Player.h"

class Army : public Object
{
public:
	Army(int i);
	
	~Army();

	enum A_STATE { ST_FREE, ST_TRAINING, ST_CONPLETE };
	int _citizens[5]{};
	A_STATE _a_state;	//-1 : 배정 X, 0 : 배정대기, 1 : army
	

	float _arrival_x, _arrival_y, _arrival_z;
	int _playerID;
	int _resource_amount[5] = {};

	void SpawnArmy(float x, float y, float z);
	void set_army_move();
	void set_army_arrival_location(float x, float y);
	void set_army_return_home();
	void set_army_disband();
};