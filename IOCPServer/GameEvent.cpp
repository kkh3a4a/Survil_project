#include "GameEvent.h"
#include"random"

GameEvent::GameEvent(int id)
{
	_id = id;
	ev_type = EV_TYPE::EV_FREE;
}

void GameEvent::random_create()
{
	std::uniform_int_distribution <int>map_uid{ 100, one_side_number - 100 };
	std::default_random_engine dre;


	
	retry:
	_x = map_uid(dre) * 100;
	_y = map_uid(dre) * 100;
	_z = 10;
	for (int id = 0; id < MAXPLAYER; ++id)
	{
		if (!overlap_check(id, _id, 5000))
			goto retry;
	}
	ev_type = random_ev[rand() % (static_cast<int>(EV_COUNT) - 1) + 1];
	if (ev_type == EV_FREE || ev_type == EV_COUNT)
		DebugBreak();

}

void GameEvent::do_event()
{

}
