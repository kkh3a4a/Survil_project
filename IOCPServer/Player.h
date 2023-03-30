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

	float _currentX, _currentY, _currentZ;


	Player(int id, STATE state = STATE::ST_FREE);
	~Player();

	void send_packet(void * packet);
	void set_player_location(float x, float y, float z , float sight_x, float sight_y);
	void keyinput();
};
