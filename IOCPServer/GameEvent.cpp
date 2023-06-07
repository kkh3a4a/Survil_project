#include "GameEvent.h"
#include"random"
#include"Army.h"
#include<iostream>

GameEvent::GameEvent(int id)
{
	_id = id;
	ev_type = EV_TYPE::EV_FREE;
	g_player_id = -1;
}

void GameEvent::random_create()
{
	std::uniform_int_distribution <int>map_uid{ 100, one_side_number - 100 };
	std::default_random_engine dre;
	dre.seed(rand() * _id);
	ev_type = random_ev[rand() % (static_cast<int>(EV_COUNT) - 1) + 1];
	g_player_id = -1;
	retry:
	_x = map_uid(dre) * 100;
	_y = map_uid(dre) * 100;
	_z = 10;
	for (int p_id = 0; p_id < MAXPLAYER; ++p_id)
	{
		if (!overlap_check(p_id, _id, 10000))
			goto retry;
	}
	
	if (ev_type == EV_FREE || ev_type == EV_COUNT)
		DebugBreak();

}

void GameEvent::check_event(int player_id, int army_id)
{
	//std::cout << p_id << " : check_event : "  << _id << std::endl;
	sc_packet_eventselect packet;
	int s_option = 1;
	packet.e_id = _id;
	a_id = army_id;
	g_player_id = player_id;
	packet.e_type = static_cast<int>(ev_type);
	switch (ev_type)
	{
	case GameEvent::EV_FREE:
	{
		DebugBreak();
		break;
	}
	case GameEvent::EV_GETOIL:
	{
		swprintf(packet.summary, L"석유을 발견했습니다.");
		resource_count[0] = rand() % 30 + 13;
		swprintf(packet.first, L"석유 %d 개 획득하였습니다", resource_count[0]);
		break;
	}
	case GameEvent::EV_GETWATER:
	{
		swprintf(packet.summary, L"물을 발견했습니다.");
		resource_count[1] = rand() % 30 + 13;
		swprintf(packet.first, L"물 %d 개 획득하였습니다", resource_count[1]);
		break;
	}
	case GameEvent::EV_GETIRON:
	{
		swprintf(packet.summary, L"철을 발견했습니다.");
		resource_count[2] = rand() % 30 + 13;
		swprintf(packet.first, L"철 %d 개 획득하였습니다", resource_count[2]);
		break;
	}
	case GameEvent::EV_GETFOOD:
	{
		swprintf(packet.summary, L"식량을 발견했습니다.");
		resource_count[3] = rand() % 30 + 13;
		swprintf(packet.first, L"식량 %d 개 획득하였습니다", resource_count[3]);
		break;
	}
	case GameEvent::EV_GETWOOD:
	{
		swprintf(packet.summary, L"나무를 발견했습니다.");
		resource_count[4] = rand() % 30 + 13;
		swprintf(packet.first, L"나무 %d 개 획득하였습니다", resource_count[4]);
		break;
	}
	case GameEvent::EV_GETCITIZEN:
	{
		swprintf(packet.summary, L"시민을 발견했습니다.");
		citizen_count = rand() % 5 + 13;
		resource_count[3] = rand() % 50 + 25;
		resource_count[1] = rand() % 50 + 25;
		swprintf(packet.first, L"시민 %d명을 마을까지 호위합니다.", citizen_count);
		swprintf(packet.second, L"식량 %d 물 %d 를 약탈합니다", resource_count[3], resource_count[1]);
		s_option = 2;
		break;
	}
	case GameEvent::EV_COUNT:
	{
		DebugBreak();
		break;
	}
	default:
	{
		DebugBreak();
		break;
	}
	}
	packet.type = SC_PACKET_EVENTSELECT;
	packet.size = sizeof(sc_packet_eventselect);
	packet.s_option = s_option;

	Player* player = reinterpret_cast<Player*>(objects[g_player_id]);
	player->send_packet(&packet);

}

void GameEvent::do_event(int select_num)
{
	//std::cout<< _id << " select number :  " << select_num << std::endl;
	Army* army = reinterpret_cast<Army*>(objects[a_id]);
	switch (ev_type)
	{
	case GameEvent::EV_FREE:
	{
		DebugBreak();
		break;
	}
	case GameEvent::EV_GETOIL:
	{
		if (select_num == 0)
			army->_resource_amount[0] += resource_count[0];
		break;
	}
	case GameEvent::EV_GETWATER:
	{
		if (select_num == 0)
			army->_resource_amount[1] += resource_count[1];
		break;
	}
	case GameEvent::EV_GETIRON:
	{
		if (select_num == 0)
			army->_resource_amount[2] += resource_count[2];
		break;
	}
	case GameEvent::EV_GETFOOD:
	{
		if (select_num == 0)
			army->_resource_amount[3] += resource_count[3];
		break;
	}
	case GameEvent::EV_GETWOOD:
	{
		if (select_num == 0)
			army->_resource_amount[4] += resource_count[4];
		break;
	}
	case GameEvent::EV_GETCITIZEN:
	{
		if (select_num == 0)
		{
			army->set_army_return_home();
			army->_Gypsy_citizen = citizen_count;
			army->is_escort = true;
			//시민이랑 같이 마을 이동
		}
		else if (select_num == 1)
		{
			army->_resource_amount[3] += resource_count[3];
			army->_resource_amount[1] += resource_count[1];
		}
		break;
	}
	case GameEvent::EV_COUNT:
	{
		DebugBreak();
		break;
	}
	default:
	{
		DebugBreak();
		break;
	}
	}
	remove_event();
}

void GameEvent::remove_event()
{
	ev_type = EV_FREE;
	sc_packet_removeevent packet;
	packet.type = SC_PACKET_REMOVEEVENT;
	packet.size = sizeof(packet);
	packet.e_id = _id;
	
	for (int p_id = 0; p_id < MAXPLAYER; ++p_id)
	{
		Player* player = reinterpret_cast<Player*>(objects[p_id]);
		if (player->view_list.count(_id) != 0)
		{
			player->send_packet(&packet);
		}
	}
}

void GameEvent::reset_event()
{
	for (auto& a : resource_count)
		a = 0;
	citizen_count = 0;
	a_id = 0;
	g_player_id = -1;

	std::uniform_int_distribution <int>map_uid{ 100, one_side_number - 100 };
	std::default_random_engine dre;
	dre.seed(rand() * _id);
	ev_type = random_ev[rand() % (static_cast<int>(EV_COUNT) - 1) + 1];

retry:
	_x = map_uid(dre) * 100;
	_y = map_uid(dre) * 100;
	_z = 10;
	for (int p_id = 0; p_id < MAXPLAYER; ++p_id)
	{
		if (!overlap_check(p_id, _id, 10000))
			goto retry;
	}
	ev_type = random_ev[rand() % (static_cast<int>(EV_COUNT) - 1) + 1];
	if (ev_type == EV_FREE || ev_type == EV_COUNT)
		DebugBreak();
}
