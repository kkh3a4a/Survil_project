#include "Building.h"
#include "Citizen.h"
#include "Player.h"


#include<iostream>
using namespace std;

Building::Building(int id)
{
	_id = id;
	_type = -1;
	for (auto& a : _citizens)
		a = nullptr;
}

Building::~Building()
{
}

bool Building::_create_building(float x, float y, char type,int id)
{
	bool isSuccessCreate = false;
	Player* player = reinterpret_cast<Player*>(objects[((id - BUILDINGSTART) / PLAYERBUILDINGCOUNT)]);
	player->_resource_amount[0];//0 : 석유,		1 : 물,		2 : 철,		3 : 식량,	4 : 나무
	switch (type)
	{


	case 1:
	{
		if (player->_resource_amount[4] < 10)
		{
			isSuccessCreate = false;
		}
		else
		{
			player->_resource_amount[4] -= 10;
			player->send_resource_amount();
			for(auto& a : _citizens)
			{
				// 생성될 때 시민을 배치해준다.
				if (a == nullptr)
				{
					for (int citizen_id = CITIZENSTART + player->_id * PLAYERCITIZENCOUNT; citizen_id < CITIZENSTART + (player->_id + 1) * PLAYERCITIZENCOUNT; ++citizen_id)
					{
						Citizen* citizen = reinterpret_cast<Citizen*>(objects[citizen_id]);
						if (citizen->_Job == -1)
							continue;
						if (citizen->_HouseId == -1)
						{
							citizen->_HouseId = _id;
							a = citizen;
							break;
						}
					}
				}
			}
			isSuccessCreate = true;
		}
		break;
	}
	case 2:
	{
		if (player->_resource_amount[4] < 20 || player->_resource_amount[2] < 10)
		{
			isSuccessCreate = false;
		}
		else
		{
			player->_resource_amount[4] -= 20; player->_resource_amount[2] -= 10;
			player->send_resource_amount();
			for (auto& a : _citizens)
			{
				// 생성될 때 시민을 배치해준다.
				if (a == nullptr)
				{
					for (int citizen_id = CITIZENSTART + player->_id * PLAYERCITIZENCOUNT; citizen_id < CITIZENSTART + (player->_id + 1) * PLAYERCITIZENCOUNT; ++citizen_id)
					{
						Citizen* citizen = reinterpret_cast<Citizen*>(objects[citizen_id]);
						if (citizen->_Job == -1)
							continue;
						if (citizen->_HouseId == -1)
						{
							citizen->_HouseId = _id;
							a = citizen;
							break;
						}
					}
				}
			}
			isSuccessCreate = true;
		}
		break;
	}
	case 3:
	{
		if (player->_resource_amount[4] < 30 || player->_resource_amount[2] < 20)
		{
			isSuccessCreate = false;
		}
		else
		{
			player->_resource_amount[4] -= 30; player->_resource_amount[2] -= 20;
			player->send_resource_amount();
			for (auto& a : _citizens)
			{
				// 생성될 때 시민을 배치해준다.
				if (a == nullptr)
				{
					for (int citizen_id = CITIZENSTART + player->_id * PLAYERCITIZENCOUNT; citizen_id < CITIZENSTART + (player->_id + 1) * PLAYERCITIZENCOUNT; ++citizen_id)
					{
						Citizen* citizen = reinterpret_cast<Citizen*>(objects[citizen_id]);
						if (citizen->_Job == -1)
							continue;
						if (citizen->_HouseId == -1)
						{
							citizen->_HouseId = _id;
							a = citizen;
							break;
						}
					}
				}
			}
			isSuccessCreate = true;
		}
		break;
	}
	case 11:
	{
		isSuccessCreate = true;
		break;
	}
	default:
	{
		break;
	}


	}


	if (isSuccessCreate)
	{
		_x = x;
		_y = y;
		_type = type;
	}
	return isSuccessCreate;
}

