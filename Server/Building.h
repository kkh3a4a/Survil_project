#pragma once
#include "NewGlobal.h"

class Building
{
public:
	char Type{};
	char Level{};
	II Location;
	char HP{};
	char IncludeCitizen{};
	
	Building();
	~Building();
	
};

