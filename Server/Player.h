#pragma once
#include "Building.h"
#include "Citizen.h"
#include "Resource.h"

class Player
{
public:
	Player();
	~Player();
	
	void Move();
	
private:
	int Port;
	II CurrentLocation;
	II CityLocation;
	
	Resource* ResourceList[30];
	Building* BuildingList[30];
	Citizen* CitizenList[30];

	
	keyboard_input* my_keyinput = new keyboard_input;
	
};
