#pragma once
#include"Object.h"
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
	char _name[MAXNAMESIZE + 1]{};
	char _packet_buf[BUFSIZE]{};

	float _currentX, _currentY, _currentZ;


	Player(STATE state = STATE::ST_FREE);
	~Player();

	void send_packet(void * packet);
	void set_player_location(float x, float y, float z);
};
