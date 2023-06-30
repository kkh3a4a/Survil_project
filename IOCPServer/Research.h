#pragma once




class Research
{
public:
	int tech_phase = 0;
	int lab_count = 0;
	short tech[5]{};		// 5개 종류에 배울때마다 1씩 늘어남

public:
	void set_tech_upgrade(int type, int level);
	void set_tech_phase(int phase);
}; 