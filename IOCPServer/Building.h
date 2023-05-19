#pragma once
#include "Object.h"

class Building : public Object
{
public:
	Building(int);
	~Building();
	
	char _type{};	// -1 : 아직 지어지지 않음 
					// 1 : House , 2: House2, 3: House3
					// 11 : HunterHouse
					// 21 : armycamp (정찰병)	22 : 군인
	char _hp{};
	char _level{};
	class Citizen* _citizens[5]{};
	int _citizencount;
	int _client_id;
	bool training_finish = true;
	bool _create_building(float x, float y, char type, int id);
	void set_building_citizen_placement(char isplus);
	void WorkBuilding();
	void training_amry();
};

