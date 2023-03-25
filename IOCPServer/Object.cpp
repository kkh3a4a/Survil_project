#include "Object.h"

Object::Object()
{
	x = 0.f, y = 0.f, z = 0.f;
	rx = 0.f, ry = 0.f, rz = 0.f;
	sx = 0.f, sy = 0.f, sz = 0.f;
	_id = -1; _type = OBJTYPE::NONE;
}

Object::~Object()
{

}



void Object::ResetOBJ()
{
	x = 0.f, y = 0.f, z = 0.f;
	rx = 0.f, ry = 0.f, rz = 0.f;
	sx = 0.f, sy = 0.f, sz = 0.f;
}
