#include "Resource.h"
#include"Citizen.h"
#include<iostream>
#include<random>

Resource::Resource(int id, char type)
{
	_id = id;
	_type = type;
	for (int i = 0; i < 10; ++i)
	{
		_workcitizens[i] = nullptr;
	}
}

Resource::~Resource()
{
}

void Resource::set_resource_spwan_location(float player_x, float player_y, float player_z)
{
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_int_distribution <int>resource_uid{ -5, 5 };
	do
	{
		retry:
		_x = player_x + resource_uid(dre) * 1000;
		_y = player_y + resource_uid(dre) * 1000;
		for (int i = RESOURCESTART; i < _id; ++i)
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
	int Maxdistance = -99999999;
	if (isplus)
	{
		for (int i = CITIZENSTART + client_id * PLAYERCITIZENCOUNT; i < CITIZENSTART + client_id * PLAYERCITIZENCOUNT + PLAYERCITIZENCOUNT; ++i)
		{
			Citizen* citizen = reinterpret_cast<Citizen*>(objects[i]);
			if(citizen->_job == 0)
			{
				int distance = sqrt(pow(_x - citizen->_x, 2) + pow(_y - citizen->_y, 2));
				if (Mindistance > distance)
				{
					Mindistance = distance;
					placement_citizen = citizen;
				}
			}
		}
		for(int i= 0;i<10;++i)
		{
			if(_workcitizens[i] == nullptr)
			{
				if (placement_citizen != nullptr)
				{
					float citizen_around_resource_x = _x;
					float citizen_around_resource_y = _y;
					placement_citizen->make_random_round_position(citizen_around_resource_x, citizen_around_resource_y, 300, 10, i);
					
					placement_citizen->_job = 1;
					placement_citizen->_job_x = citizen_around_resource_x;
					placement_citizen->_job_y = citizen_around_resource_y;
					placement_citizen->_job_z = _z;
					if(!IsNight)
						placement_citizen->set_citizen_arrival_location(citizen_around_resource_x, citizen_around_resource_y, _z);
					_workcitizens[i] = placement_citizen;
					_citizencount++;
					break;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 10; ++i)
		{
			if (_workcitizens[i] != nullptr)
			{
				int distance = sqrt(pow(_x - _workcitizens[i]->_x, 2) + pow(_y - _workcitizens[i]->_y, 2));
				if (Maxdistance < distance)
				{
					Maxdistance = distance;
					placement_citizen = _workcitizens[i];
				}
			}
		}
		for (int i = 0; i < 10; ++i)
		{
			if (_workcitizens[i] == placement_citizen)
			{
				if (placement_citizen != nullptr)
				{
					/*if(_workcitizens[i]->_x == _x && _workcitizens[i]->_y && !IsNight)
					{
						_workcitizens[i]->_arrival_x = _x + i * 100 - 500;
						_workcitizens[i]->_arrival_y = _y + 500;
					}
					else
					{
						_workcitizens[i]->_arrival_x = _workcitizens[i]->_x;
						_workcitizens[i]->_arrival_y = _workcitizens[i]->_y;
					}*/
					
					/*_workcitizens[i]->_arrival_x = _x + i * 100 - 500;
					_workcitizens[i]->_arrival_y = _y + 500;*/
					Player* player = reinterpret_cast<Player*>(objects[client_id]);
					player->move_citizen_to_tower(_workcitizens[i]->_id);
					
					_workcitizens[i]->_job = 0;
					_workcitizens[i] = nullptr;
					_citizencount--;
					break;
				}
			}
		}
	}



	sc_packet_citizenplacement packet;
	packet.size = sizeof(sc_packet_citizenplacement);
	packet.type = SC_PACKET_CITIZENPLACEMENT;
	
	packet.object_id = _id;
	packet.workcitizen = _citizencount;
	Player* player = reinterpret_cast<Player*>(objects[client_id]);
	packet.playerjobless = player->joblesscitizen();

	player->send_packet(&packet);
	
}

void Resource::collect_resource()
{
	if (_type == -1)
		return;
	
	bool change_Resource_amount = false;
	int  _WorkcitizenNum = 0;
	for (int i = 0; i < 10; ++i)
	{
		if (_workcitizens[i] != nullptr)
		{
			if (_workcitizens[i]->_job == -1)
			{
				_workcitizens[i] = nullptr;
				continue;
			}
			//if (_workcitizens[i]->_x == _x && _workcitizens[i]->_y == _y && _amount > 0 )
			if(_amount > 0)
			{
				_amount--;
				Player* player = reinterpret_cast<Player*>(objects[_workcitizens[i]->_playerID]);
				player->_resource_amount[_type] += 1 * player->_work_efficiency;
				change_Resource_amount = true;
			}
		}
	}
	if (_amount <= 0)
	{
		for (int i = 0; i < 10; ++i)
		{
			if (_workcitizens[i] != nullptr)
			{
				/*_workcitizens[i]->_arrival_x = _x + i * 100 - 500;
				_workcitizens[i]->_arrival_y = _y + 200;*/
				Player* player = reinterpret_cast<Player*>(objects[_workcitizens[i]->_playerID]);
				player->move_citizen_to_tower(_workcitizens[i]->_id);
				_workcitizens[i]->_job = 0;
				_workcitizens[i] = nullptr;
			}
		}
		_x = -1;
		_y = -1;
		_type = -1;
	}

	if (change_Resource_amount)
	{
		sc_packet_resourceamount packet;
		packet.amount = _amount;
		packet.resourceid = _id;
		packet.size = sizeof(sc_packet_resourceamount);
		packet.type = SC_PACKET_RESOURCEAMOUNT;
		
		for (int i = 0; i < MAXPLAYER; ++i)
		{
			reinterpret_cast<Player*>(objects[i])->send_packet(&packet);
		}
	}
}
