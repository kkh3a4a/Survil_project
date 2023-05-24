#pragma once
#include"protocol.h"
#include"Network.h"
#include "Policy.h"

class Player : public Object
{
public:
	

	STATE _state;
	SOCKET _socket;
	WSA_OVER_EX _wsa_recv_over;
	int _prev_size{};
	char _packet_buf[BUFSIZE]{};
	char is_connected;
	char w, a, s, d;

	Policy _policy;
	
	std::set<int> view_list;

	//0 : 석유,		1 : 물,		2 : 철,		3 : 식량,	4 : 나무
	int _resource_amount[5] = {};

	float _currentX, _currentY, _currentZ;

	char** player_sight_terrain;
	float _terrainX, _terrainY, _terrainZ;

	volatile int _Minimap_terrainsend = 0;
	std::mutex _t_l;

	Player(int id, STATE state = STATE::ST_FREE);
	~Player();

	void send_packet(void * packet);
	void set_player_location(float x, float y, float z);
	void key_input(char** player_sight_terrain_line, char** player_sight_temperature_line);
	void send_resource_amount();
	void send_terrain_All();
	void send_sunangle(const float sunangle);
	int playercitizencount();
	int joblesscitizen();
	void playerMinimapLocation(float mini_x, float mini_y);
	void find_event(int e_id);

};
