#pragma once
#include<map>
#include "Player.h"

class Game
{
public:
	std::map<int, Player*> players; //port, player
	float SunAngle{};
	char CurrentPlayerNum{};
	
public:
	Game();
	~Game();
	void Update();
	void MovePlayer();
};

