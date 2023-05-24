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
	dre.seed(rand() * _id);

	
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

void GameEvent::check_event(int p_id)
{
	switch (ev_type)
	{
	case GameEvent::EV_FREE:
	{

		break;
	}
	case GameEvent::EV_GETOIL:
	{
		wcscpy(summary, L"석유을 발견했습니다.");

		break;
	}
	case GameEvent::EV_GETWATER:
	{
		wcscpy(summary, L"물을 발견했습니다.");
		break;
	}
	case GameEvent::EV_GETFOOD:
	{
		wcscpy(summary, L"식량을 발견했습니다.");
		break;
	}
	case GameEvent::EV_GETIRON:
	{
		wcscpy(summary, L"철을 발견했습니다.");
		break;
	}
	case GameEvent::EV_GETWOOD:
	{
		wcscpy(summary, L"나무를 발견했습니다.");
		break;
	}
	case GameEvent::EV_GETCITIZEN:
	{
		wcscpy(summary, L"시민을 발견했습니다.");
		break;
	}
	case GameEvent::EV_COUNT:
	{
		wcscpy(summary, L"자원을 발견했습니다.");
		break;
	}
	default:
		break;
	}

}

void GameEvent::do_event(int select_num)
{

}
