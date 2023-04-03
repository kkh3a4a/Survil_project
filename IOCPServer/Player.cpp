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
	_terrainX = 0; _terrainY = 0; _terrainZ = 0;
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

void Player::set_player_location(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = 0;
	_currentX = - (SIGHT_X * 100 / 2);
	_currentY = - (SIGHT_Y * 100 / 2);
	_currentZ = 0;
}

void Player::keyinput()
{ 
	//std::cout << _currentX <<std::endl;
	bool keyinput = false;
	if (w)
	{
		keyinput = true;
		_currentY -= 20;
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
	//terrain좌표와 current좌표를 비교하여 100차이가 나는경우 terrain좌표 업데이트
	bool ischangeTerrain =false;
	if((int)(_currentX) % 100 ==0)
	{
		if ((int)_terrainX / 100 != (int)(_currentX) / 100)
		{
			_terrainX = _currentX;
			ischangeTerrain = true;
		}
	}
	if ((int)(_currentY) % 100 == 0)
	{
		if ((int)_terrainY / 100 != (int)(_currentY / 100))
		{
			_terrainY = _currentY;
			ischangeTerrain = true;
		}
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
	if (ischangeTerrain)
	{
		sc_packet_terrainlocation packet;
		packet.size = sizeof(sc_packet_terrainlocation);
		packet.type = SC_PACKET_TERRAINLOCATION;

		packet.terrainX = _terrainX;
		packet.terrainY = _terrainY;
		send_packet(&packet);
	}
}

void Player::send_resourceacount()
{
	sc_packet_playerresource packet;
	packet.size = sizeof(sc_packet_playerresource);
	packet.type = SC_PACKET_PLAYERRESOURCE;

	for (int i = 0; i < 5; ++i)
	{
		packet.resources_acount[i] = _resourceacount[i];
	}
	send_packet(&packet);
}


