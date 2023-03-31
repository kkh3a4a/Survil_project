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


void Citizen::set_citizen_spwan_location(float x, float y, float z)
{
	_Job = 0;
	_arrival_x = _x = x;
	_arrival_y = _y = y;
	_arrival_z = _z = z;
}

void Citizen::set_citizen_arrival_location(float ax, float ay, float az)
{
	_arrival_x = ax;
	_arrival_y = ay;
	_arrival_z = az;
}

void Citizen::set_citizen_move()
{
	if (_x != _arrival_x || _y != _arrival_y)
	{

		float distance = sqrt(pow(_x - _arrival_x, 2) + pow(_y - _arrival_y, 2));
		if (distance < 20)
		{
			_x = _arrival_x;
			_y = _arrival_y;
		}
		else
		{
			_x += ((_arrival_x - _x) / distance) * 10;
			_y += ((_arrival_y - _y) / distance) * 10;
		}
		sc_packet_citizenmove packet;
		packet.x = _x;
		packet.y = _y;
		packet.z = _z;
		packet.rx = (_arrival_x - _x) / distance;
		packet.ry = (_arrival_y - _y) / distance;
		packet.rz = (_arrival_z - _z) / distance;
		packet.citizenid = _id;
		packet.size = sizeof(sc_packet_citizenmove);
		packet.type = SC_PACKET_CITIZENMOVE;

		for (int i = 0; i < MAXPLAYER; ++i)
		{
			reinterpret_cast<Player*>(objects[i])->send_packet(&packet);
		}
	}
}

