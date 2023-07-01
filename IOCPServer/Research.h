#pragma once
#include<chrono>


class Research
{
public:
	int tech_phase = 0;
	int lab_count = 0;
	short tech[5]{};		// 5개 종류에 배울때마다 1씩 늘어남;
	float efficiency = 1.0;
	std::chrono::system_clock::time_point Timer_Start;
	int player_id = 0;
	class Player* player;
public:

	Research(int p_id);


	void set_tech_upgrade(int type, int level);
	void set_tech_phase(int phase);
	void change_tech_timer(std::chrono::system_clock::time_point t);
	void change_lab_count(int i);
}; 