#include "Citizen.h"

Citizen::Citizen(int id)
{
	_type = OBJTYPE::CITIZEN;
	_Job = -1;
	_HP = 100;
	_job_x = 0; _job_y = 0; _job_z = 0;
	_id = id;
}

Citizen::~Citizen()
{

}

void Citizen::set_citizen_location(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}
