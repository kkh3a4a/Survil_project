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
	_citizenstate = 0;
	_alcoholic = 0;
	_disabled = false;
	_dissatisfaction = 0;
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
		_citizenstate = 1;
		float distance = sqrt(pow(_x - _arrival_x, 2) + pow(_y - _arrival_y, 2));
		if (distance < 20)
		{
			_x = _arrival_x;
			_y = _arrival_y;
			if (_job == 2)
			{
				_job = 0;
				Building* building = reinterpret_cast<Building*>(objects[_Job_id]);
				_Job_id = 0;

				sc_packet_buildsuccess packet;
				packet.type = SC_PACKET_BUILDSUCCESS;
				packet.size = sizeof(sc_packet_buildsuccess);

				packet.building_type = building->_type;
				packet.id = building->_id;
				packet.x = building->_x;
				packet.y = building->_y;

				_arrival_x = _x;
				_arrival_y = _y + 500;

				for (int player_num = 0; player_num < MAXPLAYER; player_num++) {	//모든 플레이어들에게 전송
					Player* this_player = reinterpret_cast<Player*>(objects[player_num]);
					this_player->send_packet(&packet);
				}
			}
			else if (_job == 21)
			{
				Building* building = reinterpret_cast<Building*>(objects[_Job_id]);
				building->_citizencount++;
				if (building->_citizencount == 5)
				{
					building->training_amry();
				}
			}
			if(!IsNight)
				_citizenstate = 0;
			else
				_citizenstate = 2;
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
		packet.citizenstate = _citizenstate;
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
	_alcoholic = 0;
	_disabled = false;
	_dissatisfaction = 0;
}

void Citizen::citizen_eat_food()
{
	Player* player = reinterpret_cast<Player*>(objects[_playerID]);
	if (player->_resource_amount[3] > 0) {		//음식이 있을 때
		bool had_meal = false;
		
		if (player->_policy.hearty_meal) {	//든든한 식사
			if (_satiety + 40 <= 100) {			//100안넘게
				player->_resource_amount[3] -= 2;
				_satiety += 40;
				had_meal = true;
			}
		}
		else if (player->_policy.soup) {	//수프 밥
			if (_satiety + 40 <= 100) {		//100안넘게
				player->_resource_amount[3] -= 1;
				_satiety += 40;
				had_meal = true;
			}
		}
		else {								//일반
			if (_satiety + 20 <= 100) {		//100안넘게
				player->_resource_amount[3] -= 1;
				_satiety += 20;
				had_meal = true;
			}
		}
		
		if (had_meal) {
			if (player->_policy.alcohol) {		//밀주
				if (_alcoholic + 10 <= 100)
					_alcoholic += 10;
			}
			if (_alcoholic >= 100) {			//알콜중독이면 hp 깎기
				_hp -= 20;
			}
		}
	}
	else {	//음식이 없을 때
		if (_satiety == 0) {
			_hp -= 20;
		}
	}
	
	if (_hp <= 0) {						//죽기
		citizen_dead();
	}
}

void Citizen::citizen_eat_water()
{
	Player* player = reinterpret_cast<Player*>(objects[_playerID]);
	if (player->_resource_amount[3] > 0){	//물 있을 때
		if (_thirsty + 20 <= 100) {		//100안넘게
			player->_resource_amount[1] -= 1;
			_thirsty += 20;
		}
	}
	else {	//물 없을 때
		if (_thirsty == 0) {
			_hp -= 25;
		}
	}
	
	if (_hp <= 0) {						//죽기
		citizen_dead();
	}
}
