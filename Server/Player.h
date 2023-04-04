#pragma once
#include "Citizen.h"

class Player
{
public:
	II CityLocation{};
	II CurrentLocation{};
	Resource Resources[5];
	Building Buildings[20];
	Citizen Citizens[100];
	
	int Port{};
	K KeyInput{};
	UII UI{};
	
public:
	Player();
	~Player();
	
private:
	
};
