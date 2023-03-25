#pragma once
#include "NewGlobal.h"

class Resource
{
public:
	Resource();
	~Resource();

	char Type{};
	II Location{};
	int Amount{};
	int MaxAmount{};
	int CitizenCount{};

};

