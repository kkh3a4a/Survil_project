#pragma once


class Object
{
public:
	Object();
	virtual ~Object();

	float _x, _y, _z;		//location
	float _rx, _ry, _rz;	//rotate
	float _sx, _sy, _sz;	//size
	int _id;

	virtual void ResetOBJ();
};