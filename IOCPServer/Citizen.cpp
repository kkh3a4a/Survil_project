#include "Citizen.h"
#include"Building.h"
#include<random>

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
	stay_tower = false;
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
		_rx = (_arrival_x - _x) / distance;
		_ry = (_arrival_y - _y) / distance;
		_rz = (_arrival_z - _z) / distance;
		
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

				Player* player = reinterpret_cast<Player*>(objects[_playerID]);
				player->_research->change_lab_count(player->_research->lab_count + 1);

				/*_arrival_x = _x;
				_arrival_y = _y + 500;*/
				//set_citizen_arrival_location(_x, _y + 500, _z);
				player->move_citizen_to_tower(_id);
				distance = sqrt(pow(_x - _arrival_x, 2) + pow(_y - _arrival_y, 2));

				for (int player_num = 0; player_num < MAXPLAYER; player_num++) {	//모든 플레이어들에게 전송
					Player* this_player = reinterpret_cast<Player*>(objects[player_num]);
					this_player->send_packet(&packet);
					//cout << "Type: " << (int)packet.building_type << " " << packet.id << endl;
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
			{
				_citizenstate = 0;
				if(_job > 0)
					_citizenstate = 3;
			}
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
		packet.rx = _rx;
		packet.ry = _ry;
		packet.rz = _rz;
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
	cout << "죽는 시민: " << _id << " HP: " << _hp << " Satiety: " << _satiety << " Thirst: " << _thirsty << " Temperature: " << _temperature << endl;
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
	Player* player = reinterpret_cast<Player*>(objects[_playerID]);
	player->dead_citizen_num += 1;

	player->modify_dissatisfaction(0.01);
}

void Citizen::citizen_eat_food()
{
	if (_job == 22) return;	//군인은 밥도 안먹는다
	
	Player* player = reinterpret_cast<Player*>(objects[_playerID]);
	if (_satiety > 70 && _hp > 70) return;

	if (player->_resource_amount[3] > 1) {		//음식이 있을 때
		player->_resource_amount[3] -= 1;
		modify_satiety(30);
		player->modify_dissatisfaction(-0.001);
		modify_hp(30);
	}
	else {	//음식이 없을 때
		modify_satiety(-20);
	}
	
	if (_satiety == 0) {
		player->modify_dissatisfaction(0.005);
		modify_hp(-20);
	}
	if (_alcoholic >= 100) {			//알콜중독이면 hp 깎기
		modify_hp(-10);
	}
}

void Citizen::citizen_drink_water()
{
	if (_job == 22) return;	//군인은 물도 안마신다

	Player* player = reinterpret_cast<Player*>(objects[_playerID]);
	if (_thirsty > 70 && _hp > 70) return;

	if (player->_resource_amount[1] > 0){	//물 있을 때
		player->_resource_amount[1] -= 1;
		modify_thirsty(30);
		player->modify_dissatisfaction(-0.001);
		modify_hp(30);
	}
	else {	//물 없을 때
		modify_thirsty(-20);
	}
	
	if (_thirsty == 0) {
		player->modify_dissatisfaction(0.05);
		modify_hp(-25);
	}
}

void Citizen::modify_hp(int amount)
{
	if (_hp + amount >= 100) _hp = 100;
	else if (_hp + amount <= 0) citizen_dead();
	else _hp += amount;
	//cout << "Hp: " << (int)_hp << endl;
}

void Citizen::modify_satiety(int amount)
{
	if (_satiety + amount >= 100) _satiety = 100;
	else if (_satiety + amount <= 0) _satiety = 0;
	else _satiety += amount;
}

void Citizen::modify_thirsty(int amount)
{
	if (_thirsty + amount >= 100) _thirsty = 100;
	else if (_thirsty + amount <= 0) _thirsty = 0;
	else _thirsty += amount;
}

void Citizen::make_random_round_position(float& x, float& y, float distance, int max_citizen, int iter)
{
	float angle = 3.141592 * 2 / max_citizen * iter;

	x += distance * std::cos(angle);
	y += distance * std::sin(angle);
}
