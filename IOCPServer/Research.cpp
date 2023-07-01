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
	case 2:
	{
		efficiency = 1.1;
		break;
	}
	case 3:
	{
		efficiency = 1.2;
		break;
	}
	default:
	{
		if (i > 3)
		{
			efficiency += 0.05;
		}
		break;
	}
	}
	printf("before : %d", (std::chrono::duration_cast<std::chrono::seconds>(Timer_Start - std::chrono::system_clock::now())).count());
	auto temp_timer = Timer_Start;
	
	float a = (float)std::chrono::duration_cast<std::chrono::seconds>(Timer_Start - std::chrono::system_clock::now()).count();
	float b = (efficiency - temp_efficiency);
	auto temp_timer2 = Timer_Start -
		std::chrono::seconds((int)(a * b));

	change_tech_timer(temp_timer2);
	printf("after : %d", (std::chrono::duration_cast<std::chrono::seconds>(Timer_Start - std::chrono::system_clock::now())).count());
}


void Research::set_tech_upgrade(int type, int level)
{
	if((std::chrono::duration_cast<std::chrono::seconds>(Timer_Start - std::chrono::system_clock::now())).count() < 0)
	{

		if (tech[type] == level - 1)
		{
			change_tech_timer((std::chrono::system_clock::now() + std::chrono::seconds( (int)((100.0) * (float)level / efficiency))));
			tech[type] = level;
		}
	}
}

void Research::set_tech_phase(int phase)
{
	if (tech_phase == phase - 1)
	{
		tech_phase = phase;
	}
}


