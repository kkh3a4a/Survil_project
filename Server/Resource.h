#pragma once
#include "Building.h"

class Resource
{
public:
	char Type{};///////////////0 : 석유,		1 : 물,		2 : 철,		3 : 식량,	4 : 나무
	int Amount{};
	II Location{};
	int CitizenCount{};

	Resource();
	~Resource();
};

