#pragma once
#include "NewGlobal.h"

class Building
{
public:
	Building();
	~Building();
	
	char Type{};
	II Location;
	char HP{};
	char Level{};
	char IncludeCitizen[10];
	
};

