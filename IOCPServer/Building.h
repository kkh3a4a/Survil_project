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
	class Citizen* _citizens[5]{};
	int _citizencount;
	int _client_id;
	bool activated = false;
	
	bool training_finish = true;
	int training_army_id = -1;
	int _army_type = {};

	bool _create_building(float x, float y, char type, int id);
	void set_building_citizen_placement(char isplus, int armytype);
	void WorkBuilding();
	void training_amry();
};

