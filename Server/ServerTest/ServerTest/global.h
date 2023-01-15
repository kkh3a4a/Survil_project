#pragma once
#include <cmath>

typedef struct three_float {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
}TF;

typedef struct FActor_location_rotation {
	TCHAR name[30];
	TF location;
	TF rotation;
}FActor;

typedef struct location_rotation
{
	TF location;
	TF rotation;
}location_rotation;

void FActor_TF_define(TF& a, TF& b)
{
	a.x = b.x;
	a.y = b.y;
	a.z = b.z;
}

float location_distance(TF& p1, TF& p2)
{
	float distance;

	// 피타고라스의 정리
	// pow(x,2) x의 2승,  sqrt() 제곱근
	distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

	return distance;
}

void Move_Civil(TF& civil, TF& end_location) {
	float distance;
	distance = location_distance(civil, end_location);

	civil.x += ((end_location.x - civil.x) / distance) * 20;
	civil.y += ((end_location.y - civil.y) / distance) * 20;
	civil.z += ((end_location.z - civil.z) / distance) * 20;
}