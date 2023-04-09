#pragma once
#include"protocol.h"
#include"Network.h"

class Player : public Object
{
public:
	enum class STATE{ ST_FREE, ST_CONNECT, ST_INGAME, ST_DISCONNECT };


	STATE _state;
	SOCKET _socket;
	WSA_OVER_EX _wsa_recv_over;
	int _prev_size{};
	char _packet_buf[BUFSIZE]{};
	char isconnect;
	char w, a, s, d;
	
	int _resource_amount[5] = {};

	float _currentX, _currentY, _currentZ;

	char** player_sight_terrain;
	float _terrainX, _terrainY, _terrainZ;

	Player(int id, STATE state = STATE::ST_FREE);
	~Player();

	void send_packet(void * packet);
	void set_player_location(float x, float y, float z);
	void key_input(char** player_sight_line);
	void send_resource_amount();
	void send_terrain_All();
	void send_sunangle(const float sunangle);
	void send_terrain_line();
};
