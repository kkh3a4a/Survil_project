#pragma once
#include "Object.h"

class Building : public Object
{
public:
	Building(int);
	~Building();
	
	char _type{};	// -1 : 아직 지어지지 않음 , 0 : ?, 1 : ?, 2 : ?
	char _hp{};
	char _level{};
	
};

