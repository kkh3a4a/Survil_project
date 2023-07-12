#include "Army.h"
#include "GameEvent.h"
#include"Player.h"
#include"Citizen.h"
#include<iostream>
Army::Army(int i)
{
	_id = i;
	_a_state = A_STATE::ST_FREE;
	_playerID = (i - ARMYSTART) / (ARMYMAX / 5);
	for (auto& a : _citizens)
	{
		a = -1;
	}
	_arrival_x = _x = 0;
	_arrival_y = _y = 0;
	_arrival_z = _z = 0;
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
			Player* player = reinterpret_cast<Player*>(objects[_playerID]);
			_x += ((_arrival_x - _x) / distance) * 10 * player->_adventure_speed;
			_y += ((_arrival_y - _y) / distance) * 10 * player->_adventure_speed;
		}
		int _i_x = _x; int _i_y = _y;
		_arrival_z = ((float)object_z[_i_x / 100][_i_y / 100] * (1 - ((_i_x) % 100) / 100) + (float)object_z[(_i_x + 100) / 100][_i_y / 100] * (((_i_x / 100) % 100) / 100) + (float)object_z[_i_x / 100][_i_y / 100] * (1 - ((_i_y) % 100) / 100) + (float)object_z[_i_x / 100][(_i_y + 100) / 100] * ((_i_y) % 100) / 100) * 20;
		//if (abs(_arrival_z - _z) > 10)
		{
			_z = _arrival_z;
		}
		for (int e_id = EVENTSTART; e_id < EVENTSTART + EVENTMAX; ++e_id)
		{
			GameEvent* g_event = reinterpret_cast<GameEvent*>(objects[e_id]);
			if(g_event->ev_type != GameEvent::EV_FREE && g_event->ev_type != GameEvent::EV_COUNT)
			{
				if (object_find_check(e_id, _id, 10000))
				{
					Player* player = reinterpret_cast<Player*>(objects[_playerID]);
					if (object_find_check(e_id, _id, 500) && g_event->g_player_id == -1)
					{
						g_event->check_event(_playerID, _id);
					}
					else
						player->find_event(e_id);
				}
				/*if (g_event->ev_type == g_event->EV_GETCITIZEN)
				{
					Player* player = reinterpret_cast<Player*>(objects[0]);
					player->find_event(g_event->_id);
				}*/
			}
		}
		if (object_find_check(_id, _playerID, 5500))
		{
			Player* player = reinterpret_cast<Player*>(objects[_playerID]);

			for(int i=0;i < 5;++i)
			{
				if (_resource_amount[i] > 0)
				{
					player->_resource_amount[i] += _resource_amount[i];
					_resource_amount[i] = 0;
				}
			}
			if (_Gypsy_citizen > 0)
			{
				player->create_citizen(_Gypsy_citizen);
				_Gypsy_citizen = 0;
				is_escort = false;
			}
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

void Army::set_army_return_home()
{
	float distance = sqrt(pow(_x - objects[_playerID]->_x, 2) + pow(_y - objects[_playerID]->_y, 2));
	float ar_x = objects[_playerID]->_x - ((objects[_playerID]->_x - _x) / distance) * 5000;
	float ar_y = objects[_playerID]->_y - ((objects[_playerID]->_y - _y) / distance) * 5000;
	set_army_arrival_location(ar_x, ar_y);
}

void Army::set_army_disband()
{
	sc_packet_armydisband packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_ARMYDISBAND;

	packet.issuccess = object_find_check(_id, _playerID, 6500);
	packet.a_id = _id;

	_Gypsy_citizen = 0;
	if (packet.issuccess == false)
	{
		reinterpret_cast<Player*>(objects[_playerID])->send_packet(&packet);
	}
	else
	{
		_a_state = ST_FREE;

		for (int i = 0; i < MAXPLAYER; ++i)
		{
			int x_count = 0;
			for (auto& a : _citizens)
			{
				sc_packet_citizencreate c_packet;
				c_packet.citizenid = a;
				c_packet.size = sizeof(c_packet);
				c_packet.type = SC_PACKET_CITIZENCREATE;
				objects[a]->_x = c_packet.x = _x + 200 * (x_count - 2);
				objects[a]->_y = c_packet.y = _y;
				objects[a]->_z = c_packet.z = 0;
				Citizen* citizen = reinterpret_cast<Citizen*>(objects[a]);
				citizen->set_citizen_arrival_location(c_packet.x, c_packet.y, 0);
				citizen->_job = 0;
				x_count++;
				reinterpret_cast<Player*>(objects[i])->send_packet(&c_packet);
			}
			reinterpret_cast<Player*>(objects[i])->send_packet(&packet);
		}
	}
}


