#include "Army.h"
#include<iostream>
Army::Army(int i)
{
	_id = i;
	_a_state = A_STATE::ST_FREE;
	_playerID = (i - ARMYSTART) / ARMYMAX;
	for (auto& a : _citizens)
	{
		a = -1;
	}
}

Army::~Army()
{
	_id = -1;
	_a_state = A_STATE::ST_FREE;
	for (auto& a : _citizens)
	{
		a = -1;
	}
}

void Army::SpawnArmy(float x, float y, float z)
{
	_arrival_x = _x = x;
	_arrival_y = _y = y;
	_arrival_z = _z = z;
}

void Army::set_army_move()
{
	if (_x != _arrival_x || _y != _arrival_y)
	{
		char state = 1;
		float distance = sqrt(pow(_x - _arrival_x, 2) + pow(_y - _arrival_y, 2));
		if (distance < 20)
		{
			_x = _arrival_x;
			_y = _arrival_y;
			state = 0;
		}
		bool _isOverlap = false;
		
		if (!_isOverlap)
		{
			_x += ((_arrival_x - _x) / distance) * 10;
			_y += ((_arrival_y - _y) / distance) * 10;
		}
		int _i_x = _x; int _i_y = _y;
		_arrival_z = ((float)object_z[_i_x / 100][_i_y / 100] * (1 - ((_i_x) % 100) / 100) + (float)object_z[(_i_x + 100) / 100][_i_y / 100] * (((_i_x / 100) % 100) / 100) + (float)object_z[_i_x / 100][_i_y / 100] * (1 - ((_i_y) % 100) / 100) + (float)object_z[_i_x / 100][(_i_y + 100) / 100] * ((_i_y) % 100) / 100) * 20;
		//if (abs(_arrival_z - _z) > 10)
		{
			_z = _arrival_z;
		}
		
		//_z = (object_z[_i_x / 100][_i_y / 100]) * 50;
		sc_packet_armymove packet;
		packet.x = _x;
		packet.y = _y;
		packet.z = _z;
		packet.rx = (_arrival_x - _x) / distance;
		packet.ry = (_arrival_y - _y) / distance;
		packet.rz = 0;
		packet.a_id = _id;
		packet.a_state = state;
		packet.size = sizeof(sc_packet_armymove);
		packet.type = SC_PACKET_ARMYMOVE;

		for (int i = 0; i < MAXPLAYER; ++i)
		{
			reinterpret_cast<Player*>(objects[i])->send_packet(&packet);
		}
	}
}

void Army::set_army_arrival_location(float x, float y)
{
	int temp_x = x;
	int temp_y = y;
	_arrival_x = temp_x;
	_arrival_y = temp_y;
}
