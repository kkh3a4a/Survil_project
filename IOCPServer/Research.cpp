#include "Research.h"
#include "Player.h"
#include<time.h>



Research::Research(int p_id)
{
	player_id = p_id;
	Timer_Start = std::chrono::system_clock::now();
}

void Research::change_tech_timer(std::chrono::system_clock::time_point t)
{
	Timer_Start = t;

	sc_packet_techtimer packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_TECHTIMER;
	packet.tech_timer_second = (std::chrono::duration_cast<std::chrono::seconds>(Timer_Start - std::chrono::system_clock::now())).count();
	player = reinterpret_cast<Player*>(objects[player_id]);
	player->send_packet(&packet);
}

void Research::change_lab_count(int i)
{
	lab_count = i;
	float temp_efficiency = efficiency;
	switch (i)
	{
	case 1:
	{
		efficiency = 1.0;
		break;
	}
	default:
	{
		if (i > 0)
		{
			efficiency += pow((1.0 / i), 2);
		}
		break;
	}
	}
	auto temp_timer = Timer_Start;
	
	float a = (float)std::chrono::duration_cast<std::chrono::seconds>(Timer_Start - std::chrono::system_clock::now()).count();
	float b = (efficiency - temp_efficiency);
	auto temp_timer2 = Timer_Start -
		std::chrono::seconds((int)(a * b));

	change_tech_timer(temp_timer2);
}


void Research::set_tech_upgrade(int type, int level)
{
	player = reinterpret_cast<Player*>(objects[player_id]);
	if((std::chrono::duration_cast<std::chrono::seconds>(Timer_Start - std::chrono::system_clock::now())).count() <= 0)
	{

		if (tech[type] == level - 1)
		{
			if (player->_resource_amount[2] > level * 200 && player->_resource_amount[4] > level * 200)
			{
				player->_resource_amount[2] -= level * 200;
				player->_resource_amount[4] -= level * 200;
				player->send_resource_amount();
				auto t = std::chrono::seconds((int)((100.0) * (float)level));
				change_tech_timer((std::chrono::system_clock::now() + t - std::chrono::duration_cast<std::chrono::seconds>(t * (efficiency - 1))));
				tech[type] = level;

				switch (type)
				{
				case 0: // 수집 효율화
				{
					if (level == 1)
						player->_adventure_efficiency = 1.1;
					else if (level == 2)
						player->_adventure_efficiency = 1.2;
					else if (level == 3)
						player->_adventure_efficiency = 1.3;
					else if (level == 4)
						player->_adventure_efficiency = 1.5;
					break;
				}
				case 1: // 작업 효율화
				{
					if (level == 1)
						player->_work_efficiency = 1.1;
					else if (level == 2)
						player->_work_efficiency = 1.2;
					else if (level == 3)
						player->_work_efficiency = 1.3;
					else if (level == 4)
						player->_work_efficiency = 1.5;
					break;
				}
				case 2: // 이동 경량화
				{
					if (level == 1)
						player->_adventure_speed = 1.05;
					else if (level == 2)
						player->_adventure_speed = 1.1;
					else if (level == 3)
						player->_adventure_speed = 1.15;
					else if (level == 4)
						player->_adventure_speed = 1.2;
					break;
				}
				case 3: // 시추기 효율화
				{
					if (level == 1)
						player->_oil_efficiency = 0.9;
					else if (level == 2)
						player->_oil_efficiency = 0.8;
					else if (level == 3)
						player->_oil_efficiency = 0.75;
					else if (level == 4)
						player->_oil_efficiency = 0.7;
					break;
				}
				case 4: // 건물 단열화
				{
					if (level == 1)
						player->_building_insulation = 0.9;
					else if (level == 2)
						player->_building_insulation = 0.8;
					else if (level == 3)
						player->_building_insulation = 0.75;
					else if (level == 4)
						player->_building_insulation = 0.6;
					break;
				}

				default:
					break;
				}






			}
		}
	}
}

void Research::set_tech_phase(int phase)
{
	player = reinterpret_cast<Player*>(objects[player_id]);
	if ((std::chrono::duration_cast<std::chrono::seconds>(Timer_Start - std::chrono::system_clock::now())).count() < 0)
	{

		if (tech_phase == phase - 1)
		{
			if (player->_resource_amount[0] > phase * 200 && player->_resource_amount[2] > phase * 200 && player->_resource_amount[4] > phase * 200)
			{
				
				player->_resource_amount[0] -= phase * 200;
				player->_resource_amount[2] -= phase * 200;
				player->_resource_amount[4] -= phase * 200;
				player->send_resource_amount();

				auto t = std::chrono::seconds((int)((100.0) * (float)(phase + 0.5)));
				change_tech_timer((std::chrono::system_clock::now() + t - std::chrono::duration_cast<std::chrono::seconds>(t * (efficiency - 1))));

				tech_phase = phase;
			}
		}
	}
}


