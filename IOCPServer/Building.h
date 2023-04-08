#pragma once
#include "Object.h"

class Building : public Object
{
public:
	Building(int);
	~Building();
	
	char _type{};
	char _hp{};
	char _level{};
	
};

