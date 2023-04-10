#include "Player.h"
#include <iostream>
#include <string>
#include <random>

Player::Player(int id, STATE state)
{
	_x = 0; _y = 0; _z = 0;
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
	//_x는 가운데 위치, _x +_currentX 모서리 위치
}

void Player::key_input(char** player_sight_line)
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
	bool ischangeTerrainX = false;
	bool ischangeTerrainY = false;
	if((int)(_currentX) % 100 ==0)
	{
		if ((int)_terrainX / 100 != (int)(_currentX) / 100)
		{
			ischangeTerrainX = true;
		}
	}
	if ((int)(_currentY) % 100 == 0)
	{
		if ((int)_terrainY / 100 != (int)(_currentY / 100))
		{
			ischangeTerrainY = true;
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

	if (ischangeTerrainX)
	{
		sc_packet_terrainXlocation packet;
		packet.size = sizeof(sc_packet_terrainXlocation);

		packet.type = SC_PACKET_TERRAINXLOCATION;

		packet.terrainX = _currentX;

		if (_terrainX > _currentX)
		{
			if (ischangeTerrainY)
			{
				if(_terrainY > _currentY)
				{
					memcpy(packet.terrainline_Y, player_sight_line[2] + 2, SIGHT_Y);
				}
				else
				{
					memcpy(packet.terrainline_Y, player_sight_line[2], SIGHT_Y);
				}
			}
			else
			{
				memcpy(packet.terrainline_Y, player_sight_line[2] + 1, SIGHT_Y);
			}
		}
		else if (_terrainX < _currentX)
		{
			if (ischangeTerrainY)
			{
				if (_terrainY > _currentY)
				{
					memcpy(packet.terrainline_Y, player_sight_line[3] + 2, SIGHT_Y);
				}
				else
				{
					memcpy(packet.terrainline_Y, player_sight_line[3], SIGHT_Y);
				}
			}
			else
			{
				memcpy(packet.terrainline_Y, player_sight_line[3] + 1, SIGHT_Y);
			}
		}

		send_packet(&packet);
	}
	if (ischangeTerrainY)
	{
		sc_packet_terrainYlocation packet;
		packet.size = sizeof(sc_packet_terrainYlocation);

		packet.type = SC_PACKET_TERRAINYLOCATION;

		packet.terrainY = _currentY;

		if (_terrainY > _currentY)
		{
			if (ischangeTerrainX)
			{
				if (_terrainX > _currentX)
				{
					memcpy(packet.terrainline_X, player_sight_line[0] + 2, SIGHT_X);
				}
				else
				{
					memcpy(packet.terrainline_X, player_sight_line[0], SIGHT_X);
				}
			}
			else
			{
				memcpy(packet.terrainline_X, player_sight_line[0] + 1, SIGHT_X);
			}
		}
		else if (_terrainY < _currentY)
		{
			if (ischangeTerrainX)
			{
				if (_terrainX > _currentX)
				{
					memcpy(packet.terrainline_X, player_sight_line[1] + 2, SIGHT_X);
				}
				else
				{
					memcpy(packet.terrainline_X, player_sight_line[1], SIGHT_X);
				}
			}
			else
			{
				memcpy(packet.terrainline_X, player_sight_line[1] + 1, SIGHT_X);
			}
		}



		send_packet(&packet);
	}
	if ((int)(_currentX) % 100 == 0)
	{
		if ((int)_terrainX / 100 != (int)(_currentX) / 100)
		{
			_terrainX = _currentX;
		}
	}
	if ((int)(_currentY) % 100 == 0)
	{
		if ((int)_terrainY / 100 != (int)(_currentY / 100))
		{
			_terrainY = _currentY;
		}
	}
	
	

	delete player_sight_line[0];
	delete player_sight_line[1];
	delete player_sight_line[2];
	delete player_sight_line[3];
}

void Player::send_resource_amount()
{
	sc_packet_playerresource packet;
	packet.size = sizeof(sc_packet_playerresource);
	packet.type = SC_PACKET_PLAYERRESOURCE;

	for (int i = 0; i < 5; ++i)
	{
		packet.resources_acount[i] = _resource_amount[i];
	}
	send_packet(&packet);
}

void Player::send_terrain_All()
{

	return;
}

void Player::send_sunangle(const float sunangle)
{
	sc_packet_sunangle packet;
	packet.size = sizeof(sc_packet_sunangle);
	packet.type = SC_PACKET_SUNANGLE;
	packet.sunangle = sunangle;

	send_packet(&packet);
}
