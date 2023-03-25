#include "Player.h"


Player::Player(STATE state)
{
	x = 0; y = 0; z = 0;
	_type = OBJTYPE::PLAYER;
	_state = state;
	_cameraX = 0; _cameraY = 0; _cameraZ = 0;
}

Player::~Player()
{

}

void Player::_CameraMove(const Key keyinput)
{
	int movement = 100;
	if (keyinput.W && _cameraY - movement > SIGHT_Y * UNIT)
	{
		_cameraY -= movement;
	}
	if (keyinput.S && _cameraY + movement < (one_side_number - SIGHT_Y) * UNIT)
	{
		_cameraY += movement;
	}
	if (keyinput.A && _cameraX - movement > SIGHT_X * UNIT)
	{
		_cameraX -= movement;
	}
	if (keyinput.D && _cameraX + movement < (one_side_number - SIGHT_X) * UNIT)
	{
		_cameraX += movement;
	}
}

TF Player::_getPlayerLocation()
{
	TF player_location{ x,y,z };
	return player_location;
}

void Player::_setPlayerLocation(TF player_location)
{
	x = player_location.x;
	y = player_location.y;
	z = player_location.z;
}
