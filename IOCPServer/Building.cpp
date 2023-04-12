#include "Building.h"
#include "Citizen.h"
#include "Player.h"


#include<iostream>
using namespace std;

Building::Building(int id)
{
	_id = id;
	_type = -1;
}

Building::~Building()
{
}

bool Building::_create_building(float x, float y, char type,int id)
{
	bool isSuccessCreate = false;
	Player* player = reinterpret_cast<Player*>(objects[((id - BUILDINGSTART) / PLAYERBUILDINGCOUNT)]);
	cout << player->_id << " : " << id << endl;
	if (type == 1)
	{
		isSuccessCreate = true;
	}
	else if (type == 2)
	{
		isSuccessCreate = true;
	}
	else if (type == 3)
	{
		isSuccessCreate = true;
	}


	if (type == 11)
	{
		isSuccessCreate = true;
	}


	if (isSuccessCreate)
	{
		_x = x;
		_y = y;
		_type = type;
	}
	return isSuccessCreate;
}

