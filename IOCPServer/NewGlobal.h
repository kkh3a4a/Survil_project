#pragma once

#define FIRSTSPAWN 10
#define SIGHT_X 240
#define SIGHT_Y 100
#define UNIT 100
const int one_side_number = 32000;
const int random_array_size = 90000000;
const int city_size = 100;

typedef struct two_int {
	int x;
	int y;
} II;

typedef struct three_float {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
}TF;

typedef struct Key {
	bool W;
	bool A;
	bool S;
	bool D;
}K;


II player_sight_size{ SIGHT_X, SIGHT_Y };

float location_distance(TF p1, TF p2)
{
	float distance;

	// 피타고라스의 정리
	// pow(x,2) x의 2승,  sqrt() 제곱근
	distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

	return distance;
}

float location_distance(float x1, float y1, float x2, float y2)
{
	float distance;

	// 피타고라스의 정리
	// pow(x,2) x의 2승,  sqrt() 제곱근
	distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));

	return distance;
}