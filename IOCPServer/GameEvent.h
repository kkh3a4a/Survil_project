#pragma once

#include "Player.h"
#include "GameEvent.h"

class GameEvent : public Object
{
public:
	GameEvent(int id);

	int resource_count[5] = {};
	int citizen_count = 0;
	int a_id = 0;
	int g_player_id = 0;
	enum EV_TYPE
	{
		EV_FREE,
		EV_GETOIL,
		EV_GETWATER,
		EV_GETFOOD,
		EV_GETIRON,
		EV_GETWOOD,
		EV_GETCITIZEN,
		EV_COUNT
	};

	EV_TYPE random_ev[static_cast<int>(EV_COUNT)] = {
		EV_FREE,
		EV_GETOIL,
		EV_GETWATER,
		EV_GETFOOD,
		EV_GETIRON,
		EV_GETWOOD,
		EV_GETCITIZEN
	};

	EV_TYPE ev_type;

	void random_create();
	void check_event(int p_id, int a_id);
	void do_event(int select_num);
	void remove_event();
	void reset_event();
	
};
