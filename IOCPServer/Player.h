#pragma once
#include"Object.h"

class Player : public Object
{
public:
	enum class STATE{ ST_FREE, ST_CONNECT, ST_INGAME, ST_DISCONNECT };
	Player(STATE state = STATE::ST_FREE);
	~Player();


	STATE _state;
	float _cameraX, _cameraY, _cameraZ;
	void _CameraMove(const Key keyinput);
	TF _getCameraLocation();
	TF _getPlayerLocation();
	void _setPlayerLocation(TF);
};
