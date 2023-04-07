#pragma once
#include "Object.h"

class Building : public Object
{
public:
	Building();
	~Building();
	
	char Type{};
	char HP{};
	char Level{};
	char IncludeCitizen[10];
	
};

