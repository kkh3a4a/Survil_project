#include "Player.h"
#include <iostream>
#include <string>
#include <random>

Player::Player(int id, STATE state)
{
	_x = 0; _y = 0; _z = 0;
	_type = OBJTYPE::PLAYER;
	_state = state;
	_currentX = 0; _currentY = 0; _currentZ = 0;
	w = false, a = false, s = false, d = false;
	isconnect = false;
	_id = id;
	_socket={0};
}

Player::~Player()
{

}

void Player::send_packet(void* packet)
{
	char* buf = reinterpret_cast<char*>(packet);

	WSA_OVER_EX* _wsa_send_over = new WSA_OVER_EX(OP_SEND, buf[0], packet);

	if(_socket)
		WSASend(_socket, &_wsa_send_over->_wsabuf, 1, NULL, 0, &_wsa_send_over->_wsaover, NULL);
}

void Player::set_player_location(float x, float y, float z,float sight_x, float sight_y)
{
	_x = x;
	_y = y;
	_z = 0;
	_currentX = -sight_x * 100 / 2;
	_currentY = -sight_y * 100 / 2;
	_currentZ = 0;
}

void Player::keyinput()
{
	bool keyinput = false;
	if (w)
	{
		keyinput = true;
		_currentY -= 20;
		objects[5]->_x -= 20;
		objects[5]->_y -= 20;
	}
	if (a)
	{
		keyinput = true;
		
		_currentX -= 20;
	}
	if (s)
	{
		keyinput = true;
		_currentY += 20;
	}
	if (d)
	{
		keyinput = true;
		_currentX += 20;
	}


	if (keyinput)
	{
		sc_packet_move sc_packet_move;
		sc_packet_move.currentX = _currentX;
		sc_packet_move.currentY = _currentY;
		sc_packet_move.currentZ = _currentZ;
		sc_packet_move.size = sizeof(sc_packet_move);
		sc_packet_move.type = SC_PACKET_MOVE;
		send_packet(&sc_packet_move);

	}
}

