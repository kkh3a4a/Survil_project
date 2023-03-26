#include "Player.h"
#include <iostream>
#include <string>
#include <random>

Player::Player(STATE state)
{
	_x = 0; _y = 0; _z = 0;
	_type = OBJTYPE::PLAYER;
	_state = state;
	_cameraX = 0; _cameraY = 0; _cameraZ = 0;
}

Player::~Player()
{

}

void Player::set_player_location(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = 0;
}

