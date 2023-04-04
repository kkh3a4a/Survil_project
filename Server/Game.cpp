#include "Game.h"

Game::Game()
{
	
}

Game::~Game()
{
	
}

void Game::Update()
{
	
}

void Game::MovePlayer()
{
	int movement = 100;
	for (auto& a : players)
	{
		if (a.second->KeyInput.W && a.second->CurrentLocation.y - movement > SIGHT_Y * UNIT)
		{
			a.second->CurrentLocation.y -= movement;
		}
		if (a.second->KeyInput.S && a.second->CurrentLocation.y + movement < (one_side_number - SIGHT_Y) * UNIT)
		{
			a.second->CurrentLocation.y += movement;
		}
		if (a.second->KeyInput.A && a.second->CurrentLocation.x - movement > SIGHT_X * UNIT)
		{
			a.second->CurrentLocation.x -= movement;
		}
		if (a.second->KeyInput.D && a.second->CurrentLocation.x + movement < (one_side_number - SIGHT_X) * UNIT)
		{
			a.second->CurrentLocation.x += movement;
		}
	}
}