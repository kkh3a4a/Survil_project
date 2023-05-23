#pragma once

#include "Player.h"
#include "GameEvent.h"

class GameEvent : public Object
{
public:
	GameEvent(int id);


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
	void do_event();
	
};
