#include "Object.h"





Object::Object()
{
	_x = 0.f, _y = 0.f, _z = 0.f;
	_rx = 0.f, _ry = 0.f, _rz = 0.f;
	_sx = 0.f, _sy = 0.f, _sz = 0.f;
	_id = -1; _type = OBJTYPE::NONE;
}

Object::~Object()
{

}



void Object::ResetOBJ()
{
	_x = 0.f, _y = 0.f, _z = 0.f;
	_rx = 0.f, _ry = 0.f, _rz = 0.f;
	_sx = 0.f, _sy = 0.f, _sz = 0.f;
}
