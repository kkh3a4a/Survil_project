#include "Player.h"
#include <iostream>
#include <string>
#include <random>

Player::Player(STATE state)
{
	_x = 0; _y = 0; _z = 0;
	_type = OBJTYPE::PLAYER;
	_state = state;
	_currentX = 0; _currentY = 0; _currentZ = 0;
}

Player::~Player()
{

}

void Player::send_packet(void* packet)
{
	char* buf = reinterpret_cast<char*>(packet);

	WSA_OVER_EX* _wsa_send_over = new WSA_OVER_EX;
	ZeroMemory(&_wsa_send_over->_wsaover, sizeof(_wsa_send_over->_wsaover));
	_wsa_send_over->_iocpop = OP_SEND;
	_wsa_send_over->_wsabuf.buf = _wsa_send_over->_buf;
	_wsa_send_over->_wsabuf.len = buf[0];
	memcpy(_wsa_send_over->_buf, buf, buf[0]);

	WSASend(_socket, &_wsa_send_over->_wsabuf, 1, NULL, 0, &_wsa_send_over->_wsaover, NULL);
}

void Player::set_player_location(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = 0;
	_currentX = x;
	_currentY = y;
	_currentZ = 0;
}

