#pragma once
#include"Network.h"



class Citizen : public Object
{
public:
	Citizen(int id);
	~Citizen();

	char _Job;	//	-1 : 생성안됨, 및 사망, 0 : 무직, 1 : 자원 채취, 2 : ....
	float _job_x, _job_y, _job_z;
	char _HP;

	void set_citizen_location(float x, float y, float z);
};

