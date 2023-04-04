#include "Player.h"

Player::Player()
{
	std::random_device rd;
	std::default_random_engine dre{ rd() };
	std::uniform_int_distribution <int>TotalMapRandomLocation{ 0 + 1000, one_side_number - 1000 };
	std::uniform_int_distribution <int>ResourceRandomLocation{ -100, 100 };
	
	CityLocation.x = TotalMapRandomLocation(dre) * UNIT;
	CityLocation.y = TotalMapRandomLocation(dre) * UNIT;
	CurrentLocation = CityLocation;

	II Location{};
	for (int i = 0; i < 5; i++){
		Location.x = ResourceRandomLocation(dre) * UNIT + CityLocation.x;
		Location.y = ResourceRandomLocation(dre) * UNIT + CityLocation.y;
		Resources[i].Location = Location;
	}
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Location.x = UNIT * 10 * i + CityLocation.x;
			Location.y = UNIT * 10 * j + CityLocation.y;
			Citizens[i].Location = Location;
		}
	}
}

Player::~Player()
{

}
