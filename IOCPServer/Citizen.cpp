#include "Citizen.h"
#include"Building.h"

//추후 지울것
#include<iostream>
using namespace std;

Citizen::Citizen(int id)
{
	_job = -1;
	_hp = 100;
	_job_x = 0; _job_y = 0; _job_z = 0;
	_id = id;
	_playerID = (_id - CITIZENSTART) / 200;
	_house_id = -1;
	_satiety = 100;
	_thirsty = 100;
}

Citizen::~Citizen()
{

}

void Citizen::set_citizen_spwan_location(float x, float y, float z)
{
	_job = 0;
	_arrival_x = _x = x;
	_arrival_y = _y = y;
	_arrival_z = _z = z;
}

void Citizen::set_citizen_arrival_location(float ax, float ay, float az)
{
	_arrival_x = ax;
	_arrival_y = ay;
	_arrival_z = az;
}

void Citizen::set_citizen_move()
{
	if (_x != _arrival_x || _y != _arrival_y)
	{

		float distance = sqrt(pow(_x - _arrival_x, 2) + pow(_y - _arrival_y, 2));
		if (distance < 20)
		{
			_x = _arrival_x;
			_y = _arrival_y;
		}
		bool _isOverlap = false;
		for (int i = BUILDINGSTART; i < BUILDINGSTART + MAXBUILDING; ++i)
		{
			Building* building = reinterpret_cast<Building*>(objects[i]);

			if (building->_type != -1)
			{
				float builddistance = sqrt(pow(_x - building->_x, 2) + pow(_y - building->_y, 2));
				/*if (builddistance < 300)
				{
					builddistance;
					if (building->_y + 350 >= _y && building->_y - 350 <= _y)
					{
						if ((building->_y - _y) <= 0)
						{
							_y += 10;
						}
						else
						{
							_y -= 10;
						}
					}

					if (building->_x + 350 >= _x && building->_x - 350 <= _x)
					{

						if ((building->_x - _x) <= 0)
						{
							_x += 10;
						}
						else
						{
							_x -= 10;
						}
					}

					_isOverlap = true;
				}
			}*/
			}

		}
		if(!_isOverlap)
		{
			_x += ((_arrival_x - _x) / distance) * 10;
			_y += ((_arrival_y - _y) / distance) * 10;
		}
		sc_packet_citizenmove packet;
		packet.x = _x;
		packet.y = _y;
		packet.z = _z;
		packet.rx = (_arrival_x - _x) / distance;
		packet.ry = (_arrival_y - _y) / distance;
		packet.rz = (_arrival_z - _z) / distance;
		packet.citizenid = _id;
		packet.size = sizeof(sc_packet_citizenmove);
		packet.type = SC_PACKET_CITIZENMOVE;

		for (int i = 0; i < MAXPLAYER; ++i)
		{
			reinterpret_cast<Player*>(objects[i])->send_packet(&packet);
		}
	}
}

void Citizen::citizen_dead()
{
	
	sc_packet_citizenremove packet;
	packet.citizenid = _id;
	packet.size = sizeof(sc_packet_citizenremove);
	packet.type = SC_PACKET_CITIZENREMOVE;

	for (int i = 0; i < MAXPLAYER; ++i)
	{
		reinterpret_cast<Player*>(objects[i])->send_packet(&packet);
	}
	_job = -1;
	_hp = 100;
	_job_x = 0; _job_y = 0; _job_z = 0;
	_playerID = (_id - CITIZENSTART) / 200;
	_house_id = -1;
	_satiety = 100;
	_thirsty = 100;
	
}

bool Citizen::citizen_eat_food()
{
	Player* player = reinterpret_cast<Player*>(objects[_playerID]);
	if (player->_resource_amount[3] > 0)
	{
		player->_resource_amount[3] -= 1;
		_satiety += 30;
		return true;
	}
	return false;
}

bool Citizen::citizen_eat_water()
{
	Player* player = reinterpret_cast<Player*>(objects[_playerID]);
	if (player->_resource_amount[3] > 0)
	{
		player->_resource_amount[1] -= 1;
		_thirsty += 30;
		
		return true;
	}
	return false;
}

