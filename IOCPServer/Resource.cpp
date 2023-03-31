#include "Resource.h"
#include"Citizen.h"
#include<iostream>
#include<random>


std::uniform_int_distribution <int>resource_uid{ -100, 100 };

Resource::Resource(int id, char type)
{
	_resourceid = id;
	_type = type;
}

Resource::~Resource()
{
}

void Resource::set_resource_spwan_location(float player_x, float player_y, float player_z)
{
	std::default_random_engine dre;
	do
	{
		retry:
		_x = player_x + resource_uid(dre) * 50;
		_y = player_y + resource_uid(dre) * 50;
		for (int i = RESOURCESTART; i < _resourceid; ++i)
		{
			if (sqrt(pow(_x - objects[i]->_x, 2) + pow(_y - objects[i]->_y, 2)) < 10)
			{
				goto retry;
			}
		}
	} while (sqrt(pow(_x - player_x, 2) + pow(_y - player_y, 2)) < 2000);

}

void Resource::set_resource_citizen_placement(int client_id, char isplus)
{
	Citizen* placement_citizen = nullptr;
	int Mindistance = 99999999;
	if (isplus)
	{
		for (int i = CITIZENSTART + client_id * PLAYERCITIZENCOUNT; i < CITIZENSTART + client_id * PLAYERCITIZENCOUNT + PLAYERCITIZENCOUNT; ++i)
		{
			Citizen* citizen = reinterpret_cast<Citizen*>(objects[i]);
			if(citizen->_Job == 0)
			{
				int distance = sqrt(pow(_x - citizen->_x, 2) + pow(_y - citizen->_y, 2));
				if (Mindistance > distance)
				{
					Mindistance = distance;
					placement_citizen = citizen;
				}
			}
		}
		if (placement_citizen != nullptr)
		{
			placement_citizen->_Job = 1;
			placement_citizen->_job_x = _x;
			placement_citizen->_job_y = _y;
			placement_citizen->_job_z = _z;
			placement_citizen->set_citizen_arrival_location(_x, _y, _z);
		}
	}
}
