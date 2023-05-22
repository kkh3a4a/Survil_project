#include "Building.h"
#include "Citizen.h"
#include "Player.h"
#include"Army.h"

#include<iostream>
using namespace std;

Building::Building(int id)
{
	_id = id;
	_type = -1;
	_client_id = (_id - BUILDINGSTART) / PLAYERBUILDINGCOUNT;
	_citizencount = 0;
	for (auto& a : _citizens)
		a = nullptr;
}

Building::~Building()
{
}

bool Building::_create_building(float x, float y, char type,int id)
{
	bool is_Success_Create = false;
	bool is_citizen = false;
	Player* player = reinterpret_cast<Player*>(objects[((id - BUILDINGSTART) / PLAYERBUILDINGCOUNT)]);
	player->_resource_amount[0];//0 : 석유,		1 : 물,		2 : 철,		3 : 식량,	4 : 나무
	
	switch (type)
	{
	case 1:
	{
		if (player->_resource_amount[4] < 10)
		{
			is_Success_Create = false;
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
						if (citizen->_job == -1)
							continue;
						if (citizen->_house_id == -1)
						{
							citizen->_house_id = _id;
							a = citizen;
							break;
						}
					}
				}
			}
			is_Success_Create = true;
		}
		break;
	}
	case 2:
	{
		if (player->_resource_amount[4] < 20 || player->_resource_amount[2] < 10)
		{
			is_Success_Create = false;
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
						if (citizen->_job == -1)
							continue;
						if (citizen->_house_id == -1)
						{
							citizen->_house_id = _id;
							a = citizen;
							break;
						}
					}
				}
			}
			is_Success_Create = true;
		}
		break;
	}
	case 3:
	{
		if (player->_resource_amount[4] < 30 || player->_resource_amount[2] < 20)
		{
			is_Success_Create = false;
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
						if (citizen->_job == -1)
							continue;
						if (citizen->_house_id == -1)
						{
							citizen->_house_id = _id;
							a = citizen;
							break;
						}
					}
				}
			}
			is_Success_Create = true;
		}
		
		break;
	}
	case 11:
	{
		if (player->_resource_amount[4] < 20)
		{
			is_Success_Create = false;
		}
		else
		{
			player->_resource_amount[4] -= 20;
			player->send_resource_amount();
			is_Success_Create = true;
		}
		
		break;
	}
	case 21:
	{
		if (player->_resource_amount[4] < 20 && player->_resource_amount[2] < 20 && player->_resource_amount[0] < 20)
		{
			is_Success_Create = false;
		}
		else
		{
			player->_resource_amount[4] -= 20;
			player->send_resource_amount();
			is_Success_Create = true;
		}

		break;
	}
	default:
	{
		break;
	}


	}

	if(is_Success_Create)
	{
		Citizen* placement_citizen = nullptr;
		int Mindistance = 99999999;
		for (int c_id = CITIZENSTART + player->_id * PLAYERCITIZENCOUNT; c_id < CITIZENSTART + (player->_id + 1) * PLAYERCITIZENCOUNT; c_id++)
		{
			Citizen* citizen = reinterpret_cast<Citizen*>(objects[c_id]);
			int distance = sqrt(pow(x - citizen->_x, 2) + pow(y - citizen->_y, 2));
			if (citizen->_job == 0)
			{
				if (Mindistance > distance)
				{
					Mindistance = distance;
					placement_citizen = citizen;
				}
			}
		}
		if (placement_citizen != nullptr && placement_citizen->_job == 0)
		{
			placement_citizen->_job = 2;
			placement_citizen->_Job_id = _id;
			placement_citizen->set_citizen_arrival_location(x, y, 0.0f);
			is_citizen = true;
		}
	}


	if (is_Success_Create)
	{
		_x = x;
		_y = y;
		_type = type;
	}
	return is_Success_Create && is_citizen;
}

void Building::set_building_citizen_placement(char isplus)
{
	Citizen* placement_citizen = nullptr;
	int Mindistance = 99999999;
	int Maxdistance = -99999999;
	
	//여러명 추가
	if (_type == 21 && training_finish)
	{
		bool army_check = false;
		for (int o_id = ARMYSTART + PLAYERARMYCOUNT * _client_id; o_id < ARMYSTART + PLAYERARMYCOUNT * (1 + _client_id); ++o_id)
		{
			Army* army = reinterpret_cast<Army*>(objects[o_id]);
			if (army->_a_state == Army::ST_FREE)
			{
				army->_a_state = Army::ST_TRAINING;
				training_army_id = o_id;
				army_check = true;
				break;
			}
		}
		if (isplus && army_check)
		{
			int count = 0;
			Citizen* multi_citizens[5];
			Citizen* last_placement_citizen = nullptr;
			for(int j = 0 ; j< 5; j++)
			{
				Mindistance = 99999999;
				for (int i = CITIZENSTART + _client_id * PLAYERCITIZENCOUNT; i < CITIZENSTART + _client_id * PLAYERCITIZENCOUNT + PLAYERCITIZENCOUNT; ++i)
				{
					Citizen* citizen = reinterpret_cast<Citizen*>(objects[i]);
					if (citizen->_job == 0)
					{
						int distance = sqrt(pow(_x - citizen->_x, 2) + pow(_y - citizen->_y, 2));
						if (Mindistance > distance)
						{
							Mindistance = distance;
							placement_citizen = citizen;
							if (last_placement_citizen == placement_citizen)
							{
								placement_citizen = nullptr;
								break;
							}
							last_placement_citizen = citizen;
						}
					}
				}
				if (placement_citizen != nullptr)
				{

					multi_citizens[j] = placement_citizen;
					placement_citizen->_job = _type;
					placement_citizen = nullptr;
					count++;
				}
			}

			// army 생성 성공
			if (count == 5)
			{
				int citizen_cnt = 0;
				for (auto& a_citizen : multi_citizens)
				{
					a_citizen->_job = _type;
					a_citizen->_job_x = _x;
					a_citizen->_job_y = _y;
					a_citizen->_job_z = _z;
					a_citizen->_Job_id = _id;
					_citizens[citizen_cnt] = a_citizen;
					citizen_cnt++;
					if (!IsNight)
						a_citizen->set_citizen_arrival_location(_x, _y, _z);
				}
				training_finish = false;
			}
			// army 생성 실패
			else
			{
				for (int i=0;i<count; ++i)
				{
					multi_citizens[i]->_job = 0;
					multi_citizens[i] = nullptr;
					Army* army = reinterpret_cast<Army*>(objects[training_army_id]);
					army->_a_state = Army::ST_FREE;
					training_army_id = -1;
				}
			}
		}
	}
	else
	{
		if (isplus)
		{
			for (int i = CITIZENSTART + _client_id * PLAYERCITIZENCOUNT; i < CITIZENSTART + _client_id * PLAYERCITIZENCOUNT + PLAYERCITIZENCOUNT; ++i)
			{
				Citizen* citizen = reinterpret_cast<Citizen*>(objects[i]);
				if (citizen->_job == 0)
				{
					int distance = sqrt(pow(_x - citizen->_x, 2) + pow(_y - citizen->_y, 2));
					if (Mindistance > distance)
					{
						Mindistance = distance;
						placement_citizen = citizen;
					}
				}
			}
			for (auto& a : _citizens)
			{
				if (a == nullptr)
				{
					if (placement_citizen != nullptr)
					{
						placement_citizen->_job = _type;
						placement_citizen->_job_x = _x;
						placement_citizen->_job_y = _y;
						placement_citizen->_job_z = _z;
						if (!IsNight)
							placement_citizen->set_citizen_arrival_location(_x, _y, _z);
						a = placement_citizen;
						_citizencount++;
						break;
					}
				}
			}
		}
		else
		{
			for (auto& a : _citizens)
			{
				if (a != nullptr)
				{
					int distance = sqrt(pow(_x - a->_x, 2) + pow(_y - a->_y, 2));
					if (Maxdistance < distance)
					{
						Maxdistance = distance;
						placement_citizen = a;
					}
				}
			}
			int i = 0;
			for (auto& a : _citizens)
			{
				// 생성될 때 시민을 배치해준다.
				if (a != nullptr)
				{
					if (a == placement_citizen)
					{
						if (placement_citizen != nullptr)
						{
							if (a->_x == _x && a->_y && !IsNight)
							{
								a->_arrival_x = _x + i * 100 - 500;
								a->_arrival_y = _y + 500;
							}
							else
							{
								a->_arrival_x = a->_x;
								a->_arrival_y = a->_y;
							}
							a->_job = 0;
							a = nullptr;
							_citizencount--;
							break;
						}
					}
				}
				i++;
			}
		}
	}

	//한명씩 추가
	
	

	sc_packet_citizenplacement packet;
	packet.size = sizeof(sc_packet_citizenplacement);
	packet.type = SC_PACKET_CITIZENPLACEMENT;

	packet.object_id = _id;
	
	packet.workcitizen = _citizencount;
	cout << _citizencount << endl;
	Player* player = reinterpret_cast<Player*>(objects[_client_id]);
	packet.playerjobless = player->joblesscitizen();
	player->send_packet(&packet);
}

void Building::WorkBuilding()
{
	Player* player = reinterpret_cast<Player*>(objects[_client_id]);
	switch (_type)
	{
	case 11:
	{
		player->_resource_amount[3] += _citizencount;
		break;
	}




	default:
		break;
	}

}

void Building::training_amry()
{
	
	_citizencount = 0;
	
	cout << "traning army" << endl;
	Army* army = reinterpret_cast<Army*>(objects[training_army_id]);
	army->_a_state = army->ST_CONPLETE;

	

	for (int i = 0; i < 5; ++i)
	{
		_citizens[i]->_job = 22;
		army->_citizens[i] = _citizens[i]->_id;
	}
	sc_packet_armytraining packet;
	packet.size = sizeof(sc_packet_armytraining);
	packet.type = SC_PACKET_ARMYTRAINING;

	packet.c_id1 = _citizens[0]->_id;
	packet.c_id2 = _citizens[1]->_id;
	packet.c_id3 = _citizens[2]->_id;
	packet.c_id4 = _citizens[3]->_id;
	packet.c_id5 = _citizens[4]->_id;
	packet.x = _x;
	packet.y = _y + 500;
	packet.z = _z;
	packet.army_id = training_army_id;
	training_army_id = -1;
	army->SpawnArmy(_x, _y + 500, _z);

	for (auto& a_c : _citizens)
	{
		a_c = nullptr;
	}
	training_finish = true;

	all_player_sendpacket(&packet);
}
