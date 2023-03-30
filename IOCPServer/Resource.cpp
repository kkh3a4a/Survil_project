#include "Resource.h"
#include<random>


std::uniform_int_distribution <int>resource_uid{ -100, 100 };

Resource::Resource(int id, char type)
{
	_resourceid = id;
	_type = type;
}

Resource::~Resource()
{
}

void Resource::set_resource_spwan_location(float player_x, float player_y, float player_z)
{
	std::default_random_engine dre;
	do
	{
		retry:
		_x = player_x + resource_uid(dre) * 50;
		_y = player_y + resource_uid(dre) * 50;
		for (int i = RESOURCESTART; i < _resourceid; ++i)
		{
			if (sqrt(pow(_x - objects[i]->_x, 2) + pow(_y - objects[i]->_y, 2)) < 10)
			{
				goto retry;
			}
		}
	} while (sqrt(pow(_x - player_x, 2) + pow(_y - player_y, 2)) < 2000);

}
