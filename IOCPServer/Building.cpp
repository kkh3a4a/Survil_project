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
	player->_resource_amount[0];//0 : 석유,		1 : 물,		2 : 철,		3 : 식량,	4 : 나무
	if (type == 1)
	{
		if (player->_resource_amount[4] < 10)
		{
			isSuccessCreate = false;
		}
		else
		{
			player->_resource_amount[4] -= 10;
			player->send_resource_amount();
			isSuccessCreate = true;
		}
	}
	else if (type == 2)
	{
		if (player->_resource_amount[4] < 20 || player->_resource_amount[2] < 10)
		{
			isSuccessCreate = false;
		}
		else
		{
			player->_resource_amount[4] -= 20; player->_resource_amount[2] -= 10;
			player->send_resource_amount();
			isSuccessCreate = true;
		}
	}
	else if (type == 3)
	{
		if (player->_resource_amount[4] < 30 || player->_resource_amount[2] < 20)
		{
			isSuccessCreate = false;
		}
		else
		{
			player->_resource_amount[4] -= 30; player->_resource_amount[2] -= 20;
			player->send_resource_amount();
			isSuccessCreate = true;
		}
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

