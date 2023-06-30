#include "Research.h"

void Research::set_tech_upgrade(int type, int level)
{
	if (tech[type] == level - 1)
	{
		tech[type] = level;
	}
}

void Research::set_tech_phase(int phase)
{
	if (tech_phase == phase - 1)
	{
		tech_phase = phase;
	}
}
