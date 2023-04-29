#include "Player.h"

#include"Citizen.h"
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

	//초기 자원 지정
	for (auto& a : _resource_amount)
	{
		a = 70;
	}
}

Player::~Player()
{

}

void Player::send_packet(void* packet)
{
	char* buf = reinterpret_cast<char*>(packet);

	WSA_OVER_EX* _wsa_send_over = new WSA_OVER_EX(OP_SEND, buf[0], packet);

	if(_state == STATE::ST_INGAME)
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

void Player::key_input(char** player_sight_terrain_line, char** player_sight_temperature_line)
{ 
	//std::cout << _currentX <<std::endl;
	bool keyinput = false;
	char directionX{};
	char directionY{};
	int speed = 25;
	bool usedX{};
	bool usedY{};

	if (w) {
		directionY = -1;
		keyinput = true;
		usedY = true;
	}
	if (s) {
		directionY = 1;
		keyinput = true;
		usedY = true;
	}
	if (a) {
		directionX = -1;
		keyinput = true;
		usedX = true;
	}
	if (d) {
		directionX = 1;
		keyinput = true;
		usedX = true;
	}

	if (keyinput) {
		//if (usedX && usedY) {
		//	//speed = sqrt(pow(speed, 2) / 2);
		//	speed = 20;
		//}
		//if((int)(_x + _currentX) / 100 > 100)
		_currentX += speed * directionX;
		//if ((int)(_y + _currentY) / 100 > 100)
		_currentY += speed * directionY;
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

	//terrain좌표와 current좌표를 비교하여 100차이가 나는경우 terrain좌표 업데이트
	bool ischangeTerrainX = false;
	bool ischangeTerrainY = false;

	if (abs(abs(_terrainX) - abs(_currentX)) >= 100) {
		ischangeTerrainX = true;
	}
	if (abs(abs(_terrainY) - abs(_currentY)) >= 100) {
		ischangeTerrainY = true;
	}
	if (w && a || w && d || s && a || s && d) {
		if (ischangeTerrainX) {
			ischangeTerrainY = true;
			_currentY = int(_currentY) / 100 * 100 + int(_currentX) % 100;
		}
		if (ischangeTerrainY) {
			ischangeTerrainX = true;
			_currentX = int(_currentX) / 100 * 100 + int(_currentY) % 100;
		}
	}
	
	//std::cout << ischangeTerrainX << " " << ischangeTerrainY << std::endl;
	
	if (ischangeTerrainX)
	{
		//Terrain
		sc_packet_terrainXlocation terrain_packet;
		terrain_packet.size = sizeof(sc_packet_terrainXlocation);
		terrain_packet.type = SC_PACKET_TERRAINXLOCATION;
		terrain_packet.terrainX = _x + _currentX;
		if (_terrainX > _currentX){
			if (ischangeTerrainY){
				if(_terrainY > _currentY){
					memcpy(terrain_packet.terrainline_Y, player_sight_terrain_line[2] + 2, SIGHT_Y);
				}
				else{
					memcpy(terrain_packet.terrainline_Y, player_sight_terrain_line[2], SIGHT_Y);
				}
			}
			else{
				memcpy(terrain_packet.terrainline_Y, player_sight_terrain_line[2] + 1, SIGHT_Y);
			}
		}
		else if (_terrainX < _currentX){
			if (ischangeTerrainY){
				if (_terrainY > _currentY){
					memcpy(terrain_packet.terrainline_Y, player_sight_terrain_line[3] + 2, SIGHT_Y);
				}
				else{
					memcpy(terrain_packet.terrainline_Y, player_sight_terrain_line[3], SIGHT_Y);
				}
			}
			else{
				memcpy(terrain_packet.terrainline_Y, player_sight_terrain_line[3] + 1, SIGHT_Y);
			}
		}
		send_packet(&terrain_packet);

		
		//Temperature
		sc_packet_temperatureX temperature_packet;
		temperature_packet.terrainX = _currentX;
		if (_terrainX > _currentX) {
			if (ischangeTerrainY) {
				if (_terrainY > _currentY) {
					memcpy(temperature_packet.terrainline_Y, player_sight_temperature_line[2] + 2, SIGHT_Y);
				}
				else {
					memcpy(temperature_packet.terrainline_Y, player_sight_temperature_line[2], SIGHT_Y);
				}
			}
			else {
				memcpy(temperature_packet.terrainline_Y, player_sight_temperature_line[2] + 1, SIGHT_Y);
			}
		}
		else if (_terrainX < _currentX) {
			if (ischangeTerrainY) {
				if (_terrainY > _currentY) {
					memcpy(temperature_packet.terrainline_Y, player_sight_temperature_line[3] + 2, SIGHT_Y);
				}
				else {
					memcpy(temperature_packet.terrainline_Y, player_sight_temperature_line[3], SIGHT_Y);
				}
			}
			else {
				memcpy(temperature_packet.terrainline_Y, player_sight_temperature_line[3] + 1, SIGHT_Y);
			}
		}
		send_packet(&temperature_packet);
	}
	
	if (ischangeTerrainY)
	{
		//Terrain
		sc_packet_terrainYlocation terrain_packet;
		terrain_packet.size = sizeof(sc_packet_terrainYlocation);
		terrain_packet.type = SC_PACKET_TERRAINYLOCATION;
		terrain_packet.terrainY = _y + _currentY;

		if (_terrainY > _currentY){
			if (ischangeTerrainX){
				if (_terrainX > _currentX){
					memcpy(terrain_packet.terrainline_X, player_sight_terrain_line[0] + 2, SIGHT_X);
				}
				else{
					memcpy(terrain_packet.terrainline_X, player_sight_terrain_line[0], SIGHT_X);
				}
			}
			else{
				memcpy(terrain_packet.terrainline_X, player_sight_terrain_line[0] + 1, SIGHT_X);
			}
		}
		else if (_terrainY < _currentY){
			if (ischangeTerrainX){
				if (_terrainX > _currentX){
					memcpy(terrain_packet.terrainline_X, player_sight_terrain_line[1] + 2, SIGHT_X);
				}
				else{
					memcpy(terrain_packet.terrainline_X, player_sight_terrain_line[1], SIGHT_X);
				}
			}
			else{
				memcpy(terrain_packet.terrainline_X, player_sight_terrain_line[1] + 1, SIGHT_X);
			}
		}
		send_packet(&terrain_packet);

		//Temperature
		sc_packet_temperatureY temperature_packet;
		temperature_packet.terrainY = _currentY;
		if (_terrainY > _currentY) {
			if (ischangeTerrainX) {
				if (_terrainX > _currentX) {
					memcpy(temperature_packet.terrainline_X, player_sight_temperature_line[0] + 2, SIGHT_X);
				}
				else {
					memcpy(temperature_packet.terrainline_X, player_sight_temperature_line[0], SIGHT_X);
				}
			}
			else {
				memcpy(temperature_packet.terrainline_X, player_sight_temperature_line[0] + 1, SIGHT_X);
			}
		}
		else if (_terrainY < _currentY) {
			if (ischangeTerrainX) {
				if (_terrainX > _currentX) {
					memcpy(temperature_packet.terrainline_X, player_sight_temperature_line[1] + 2, SIGHT_X);
				}
				else {
					memcpy(temperature_packet.terrainline_X, player_sight_temperature_line[1], SIGHT_X);
				}
			}
			else {
				memcpy(temperature_packet.terrainline_X, player_sight_temperature_line[1] + 1, SIGHT_X);
			}
		}
		send_packet(&temperature_packet);
	}

	if (abs(abs(_terrainX) - abs(_currentX)) >= 100) {
		_terrainX = _currentX;
	}
	if (abs(abs(_terrainY) - abs(_currentY)) >= 100) {
		_terrainY = _currentY;
	}


	//최우선 오류 해결 요망
	
	/*delete player_sight_terrain_line[0];
	delete player_sight_terrain_line[1];
	delete player_sight_terrain_line[2];
	delete player_sight_terrain_line[3];


	delete player_sight_temperature_line[0];
	delete player_sight_temperature_line[1];
	delete player_sight_temperature_line[2];
	delete player_sight_temperature_line[3];*/
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

int Player::playercitizencount()
{
	int _citizencount = 0;
	for (int citizen_id = CITIZENSTART + _id * PLAYERCITIZENCOUNT; citizen_id < CITIZENSTART + (_id + 1) * PLAYERCITIZENCOUNT; ++citizen_id)
	{
		Citizen* citizen = reinterpret_cast<Citizen*>(objects[citizen_id]);
		if (citizen->_Job != -1)
			_citizencount++;

	}
	return _citizencount;
}

int Player::joblesscitizen()
{
	int _citizencount = 0;
	for (int citizen_id = CITIZENSTART + _id * PLAYERCITIZENCOUNT; citizen_id < CITIZENSTART + (_id + 1) * PLAYERCITIZENCOUNT; ++citizen_id)
	{
		Citizen* citizen = reinterpret_cast<Citizen*>(objects[citizen_id]);
		if (citizen->_Job == 0)
			_citizencount++;

	}
	return _citizencount;
	return 0;
}

void Player::playerMinimapLocation(float mini_x, float mini_y)
{

	_currentX = (mini_x - _x) - (SIGHT_X * 100 / 2);
	_currentY = (mini_y - _y) - (SIGHT_Y * 100 / 2);
	
retry:
	int cas_bool = _Minimap_terrainsend;
	if (cas_bool != 0)
		goto retry;
	while (CAS(&_Minimap_terrainsend, cas_bool,1)){}
	while (_Minimap_terrainsend == 0) {};

	sc_packet_move sc_packet_move;
	sc_packet_move.currentX = _currentX;
	sc_packet_move.currentY = _currentY;
	sc_packet_move.currentZ = _currentZ;
	sc_packet_move.size = sizeof(sc_packet_move);
	sc_packet_move.type = SC_PACKET_MOVE;
	send_packet(&sc_packet_move);
}


