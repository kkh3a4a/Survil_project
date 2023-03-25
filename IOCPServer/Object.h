#pragma once
#include"NewGlobal.h"

class Object
{
public:
	Object();
	virtual ~Object();

	enum class OBJTYPE{NONE, PLAYER, CITIZEN, RESOURCE, STRUCTURE};
	float x, y, z;		//location
	float rx, ry, rz;	//rotate
	float sx, sy, sz;	//size
	int _id;
	OBJTYPE _type;

	virtual void ResetOBJ();
};