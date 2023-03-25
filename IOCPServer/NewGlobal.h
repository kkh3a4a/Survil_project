#pragma once

#define MAXPLAYER 1
#define MAXCITIZEN 200
#define FIRSTSPAWN 10
#define SIGHT_X 240
#define SIGHT_Y 100
#define UNIT 100
const int one_side_number = 32000;


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