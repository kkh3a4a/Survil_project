#include <iostream>
#include <stdio.h>
#include <random>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <cuda_runtime.h>
#include <curand.h>
#include <curand_kernel.h>
#include <cooperative_groups.h>
#include "device_launch_parameters.h"
#define PI 3.1415926
using namespace std;

const int one_side_number = 96;	//39936
const int player_sight_size = 1024;	//1024 넘으면 안됨

const int max_height = 8;
const int base_floor = 1;

typedef struct two_int {
	int x;
	int y;
} II;

typedef struct two_char {
	int x;
	int y;
} CC;

typedef struct two_float {
	float x;
	float y;
} FF;

typedef struct hill_info {
	int x;
	int y;
	int radius;
	int height;
} HI;

typedef struct compare_block {
	II middle;
	II up;
	II down;
	II left;
	II right;
}CB;

random_device rd;
default_random_engine dre(rd());
uniform_int_distribution <int>hills_location(0, one_side_number);
uniform_int_distribution <int>number_of_hills_uid(one_side_number / 10, one_side_number / 10);
uniform_int_distribution <int>hill_size_uid(one_side_number / 20, one_side_number / 10);
uniform_int_distribution <int>height_uid(5, max_height);

uniform_int_distribution <int>wind_speed_uid(0, 50);
uniform_int_distribution <int>wind_angle_uid(0, 360);

__global__
void make_hills_cuda(char** terrain_array_device, HI* hill_location_device, int hill_number)
{
	II terrain;
	terrain.x = blockIdx.x * blockDim.x + threadIdx.x;
	terrain.y = blockIdx.y * blockDim.y + threadIdx.y;
	HI hill;
	int distance;
	//printf("%d %d %d\n", terrain.x, terrain.y, i);
	//terrain_array_device[terrain.x][terrain.y] = base_floor;
	for (int i = 0; i < hill_number; i++) {
		//원래 언덕 채우기
		hill.x = hill_location_device[i].x;
		hill.y = hill_location_device[i].y;
		hill.radius = hill_location_device[i].radius;
		hill.height = hill_location_device[i].height;
		distance = sqrt(pow(terrain.y - hill.y, 2) + pow(terrain.x - hill.x, 2));

		if (distance <= hill.radius) {
			terrain_array_device[terrain.x][terrain.y] += (hill.height) * (hill.radius - distance) / hill.radius;
			return;
		}
	}
	
	
	////int id = threadIdx.x + blockIdx.x * blockDim.x;
	//int my_hill = threadIdx.x;
	//int my_y = blockIdx.x;
	//int hill_location_x = hill_location_device[my_hill].x;
	//int hill_location_y = hill_location_device[my_hill].y;
	//int radius = hill_location_device[my_hill].radius;
	//int height = hill_location_device[my_hill].height;
	//int distance{};
	//for (int x = 0; x < one_side_number; x++) {
	//	if (x < 0 || x >= one_side_number)
	//		continue;
	//	distance = sqrt(pow(x - hill_location_x, 2) + pow(my_y - hill_location_y, 2));
	//	if (distance <= radius) {
	//		terrain_array_device[x][my_y] = (height - 1) * (radius - distance) / radius;
	//		if (terrain_array_device[x][my_y] > max_height) {
	//			terrain_array_device[x][my_y] = max_height;
	//		}
	//	}
	//}
}

__global__
void terrain_change_cuda(char** terrain_array_device, int height)
{
	//바람이 불면 그쪽으로 이동하게끔 수정해야 함
	//아니면 바람으로 인한 함수 따로 만들어서 중복으로 했을 때 어떤지 확인
	const int block_num = 9;
	II terrain[block_num];
	terrain[0].x = blockIdx.x * blockDim.x + threadIdx.x;	//middle
	terrain[0].y = blockIdx.y * blockDim.y + threadIdx.y;

	if (terrain_array_device[terrain[0].x][terrain[0].y] != height) {	//인자로 들어온 높이가 아니면 리턴
		return;
	}
	if (terrain_array_device[terrain[0].x][terrain[0].y] <= base_floor) {	//base floor 보다 작으면 더이상 낮추면 안됨
		return;
	}
	
	terrain[1] = { terrain[0].x, terrain[0].y - 1 };		//up
	terrain[2] = { terrain[0].x, terrain[0].y + 1 };		//down
	terrain[3] = { terrain[0].x - 1, terrain[0].y };		//left
	terrain[4] = { terrain[0].x + 1, terrain[0].y };		//right
	terrain[5] = { terrain[0].x - 1, terrain[0].y - 1 };	//up left
	terrain[6] = { terrain[0].x - 1, terrain[0].y + 1 };	//down left
	terrain[7] = { terrain[0].x + 1, terrain[0].y - 1 };	//up right
	terrain[8] = { terrain[0].x + 1, terrain[0].y + 1 };	//down right

	int height_difference = 0;
	for (int i = 1; i < block_num; i++) {	//주변 블럭들의 높이 차이를 구함
		if (terrain[i].x < 0 || terrain[i].x >= one_side_number || terrain[i].y < 0 || terrain[i].y >= one_side_number) {	//i번째 블럭이 맵 외부일때
			continue;
		}
		if (terrain_array_device[terrain[0].x][terrain[0].y] - terrain_array_device[terrain[i].x][terrain[i].y] > height_difference) {
			height_difference = terrain_array_device[terrain[0].x][terrain[0].y] - terrain_array_device[terrain[i].x][terrain[i].y];
		}
	}

	if (height_difference <= 0) {	//주변에 더 낮은 블럭이 없으면 리턴
		return;
	}

	int num_of_lowest{};
	for (int i = 1; i < block_num; i++) {
		if (terrain[i].x < 0 || terrain[i].x >= one_side_number || terrain[i].y < 0 || terrain[i].y >= one_side_number) {	//i번째 블럭이 맵 외부일때
			num_of_lowest++;
			continue;
		}
		if (terrain_array_device[terrain[0].x][terrain[0].y] - terrain_array_device[terrain[i].x][terrain[i].y] == height_difference) {
			num_of_lowest++;
		}
	}

	if (num_of_lowest == 1) {	//가장 낮은 높이가 하나면 거기로 이동
		for (int i = 1; i < block_num; i++) {
			if (terrain[i].x < 0 || terrain[i].x >= one_side_number || terrain[i].y < 0 || terrain[i].y >= one_side_number) {	//i번째 블럭이 맵 외부일때
				continue;
			}
			if (terrain_array_device[terrain[0].x][terrain[0].y] - terrain_array_device[terrain[i].x][terrain[i].y] == height_difference) {
				terrain_array_device[terrain[i].x][terrain[i].y]++;
				terrain_array_device[terrain[0].x][terrain[0].y]--;
				return;
			}
		}
	}
	if (num_of_lowest == 8 && height_difference == 1) {	//주변이 모두 높이가 같고 높이 차이가 1이면 리턴
		return;
	}
	
	char thread_seed = (terrain[0].x + terrain[0].y + abs(terrain[0].x + terrain[0].y * height)) % num_of_lowest;	//random seed made by myself
	int iter = 0;
	for (int i = 0; i < block_num; i++) {
		if (terrain[i].x < 0 || terrain[i].x >= one_side_number || terrain[i].y < 0 || terrain[i].y >= one_side_number) {	//i번째 블럭이 맵 외부일때
			if (thread_seed == iter) {	//랜덤으로 선택된 블럭으로 이동
				terrain_array_device[terrain[0].x][terrain[0].y]--;
				return;
			}
			iter++;
		}
		else if (terrain_array_device[terrain[0].x][terrain[0].y] - terrain_array_device[terrain[i].x][terrain[i].y] == height_difference) {	//가장 낮은 높이가 여러개면 랜덤으로 하나 선택
			if (thread_seed == iter) {	//랜덤으로 선택된 블럭으로 이동
				terrain_array_device[terrain[i].x][terrain[i].y]++;
				terrain_array_device[terrain[0].x][terrain[0].y]--;
				return;
			}
			iter++;
		}
	}
}

__global__
void wind_blow_cuda(char** terrain_array_device, FF wind_direction, int wind_speed)
{
	//wind_direction은 x, y중 하나는 무조건 0이여야 함
	const int block_num = 4;
	II terrain[block_num];
	terrain[0].x = blockIdx.x * blockDim.x + threadIdx.x;	//middle
	terrain[0].y = blockIdx.y * blockDim.y + threadIdx.y;
	terrain[1] = { terrain[0].x, terrain[0].y };
	
	terrain[1].x += wind_direction.x;	//이동방향의 전방 블럭
	terrain[1].y += wind_direction.y;
	terrain[2] = terrain[1];	//이동방향의 왼쪽 대각선 블럭
	terrain[3] = terrain[1];	//이동방향의 오른쪽 대각선 블럭

	
	if ((int)wind_direction.x == 0) {
		terrain[2].x -= 1;
		terrain[3].x += 1;
	}
	else if ((int)wind_direction.y == 0) {
		terrain[2].y -= 1;
		terrain[3].y += 1;
	}
	//printf("%d %d,,, %d %d, %d %d, %d %d\n", terrain[0].x, terrain[0].y, terrain[2].x, terrain[2].y, terrain[1].x, terrain[1].y, terrain[3].x, terrain[3].y);

	if (terrain[1].x < 0 || terrain[1].x >= one_side_number || terrain[1].y < 0 || terrain[1].y >= one_side_number) {	//next가 맵 외부일때
		return;
	}
	if (terrain[2].x < 0 || terrain[2].x >= one_side_number || terrain[2].y < 0 || terrain[2].y >= one_side_number) {	//next가 맵 외부일때
		return;
	}
	if (terrain[3].x < 0 || terrain[3].x >= one_side_number || terrain[3].y < 0 || terrain[3].y >= one_side_number) {	//next가 맵 외부일때
		return;
	}
	if (terrain_array_device[terrain[0].x][terrain[0].y] <= base_floor) {	//base floor 보다 작으면 더이상 낮추면 안됨
		return;
	}

	int height_difference = -10000;
	for (int i = 1; i < block_num; i++) {
		if (terrain_array_device[terrain[0].x][terrain[0].y] - terrain_array_device[terrain[i].x][terrain[i].y]  > height_difference) {
			height_difference = terrain_array_device[terrain[0].x][terrain[0].y] - terrain_array_device[terrain[i].x][terrain[i].y];	//i블럭와 0블럭의 높이 차이
		}
	}
	//낮으면 내려가고, 높을때 1칸 차이면 올라감
	int num_of_lowest{};
	for (int i = 1; i < block_num; i++) {
		if (terrain_array_device[terrain[0].x][terrain[0].y] - terrain_array_device[terrain[i].x][terrain[i].y] == height_difference) {
			num_of_lowest++;
		}
	}
	if (num_of_lowest == 1) {	//최저높이가 하나일 경우 그리로 감
		for (int i = 1; i < block_num; i++) {
			if (terrain_array_device[terrain[0].x][terrain[0].y] - terrain_array_device[terrain[i].x][terrain[i].y] == height_difference) {
				terrain_array_device[terrain[i].x][terrain[i].y]++;
				terrain_array_device[terrain[0].x][terrain[0].y]--;
				return;
			}
		}
	}
	if (num_of_lowest == 3) {	//좌,전방,우의 높이가 최저높이로 같을 때, 전방 우선 이동
		terrain_array_device[terrain[1].x][terrain[1].y]++;
		terrain_array_device[terrain[0].x][terrain[0].y]--;
	}
	//여러개일 경우 랜덤으로 함
	//쓰레드 많아서 중복처리 될수있으니까 바람 방향에서 one side num 만큼 for문으로 돌려서 cuda실행해야함
	//바람으로 넘어가는 거에 우선순위: 높이가 같을때 앞으로, 높이가 다르다면 낮은쪽으로
	
	/*if (terrain_array_device[terrain[1].x][terrain[1].y] - terrain_array_device[terrain[0].x][terrain[0].y] < 2) {
		terrain_array_device[terrain[0].x][terrain[0].y]--;
		terrain_array_device[terrain[1].x][terrain[1].y]++;
	}
	else if (terrain_array_device[terrain[2].x][terrain[2].y] - terrain_array_device[terrain[0].x][terrain[0].y] < 2) {
		terrain_array_device[terrain[0].x][terrain[0].y]--;
		terrain_array_device[terrain[2].x][terrain[2].y]++;
	}
	else if (terrain_array_device[terrain[3].x][terrain[3].y] - terrain_array_device[terrain[0].x][terrain[0].y] < 2) {
		terrain_array_device[terrain[0].x][terrain[0].y]--;
		terrain_array_device[terrain[3].x][terrain[3].y]++;
	}
	if (terrain_array_device[terrain[0].x][terrain[0].y] < 1) {
		terrain_array_device[terrain[0].x][terrain[0].y] = 1;
	}*/
}

__global__
void player_terrain_update_cuda(char** terrain_player_sight_device, HI* hill_location_device, int num_of_hills, II player_location, FF wind_direction, int wind_speed)
{
	int x = threadIdx.x;
	int y = blockIdx.x;
	int terrain_x = player_location.x + x;
	int terrain_y = player_location.y + y;

	if (terrain_x >= 0 && terrain_x <= one_side_number && terrain_y >= 0 && terrain_y <= one_side_number) {
		terrain_player_sight_device[x][y] = base_floor;
		for (int i = 0; i < num_of_hills; i++) {
			
			//원래 언덕 채우기
			int hill_location_x = hill_location_device[i].x;
			int hill_location_y = hill_location_device[i].y;
			int radius = hill_location_device[i].radius;
			int height = hill_location_device[i].height;
			int distance = sqrt(pow(terrain_y - hill_location_y, 2) + pow(terrain_x - hill_location_x, 2));

			if (distance <= radius) {
				terrain_player_sight_device[x][y] += (height) * (radius - distance) / radius;

				//언덕 깎기
				hill_location_x = hill_location_device[i].x - radius * wind_direction.x * (100 - wind_speed) / 50;
				hill_location_y = hill_location_device[i].y - radius * wind_direction.y * (100 - wind_speed) / 50;
				distance = sqrt(pow(terrain_y - hill_location_y, 2) + pow(terrain_x - hill_location_x, 2));

				if (distance <= radius) {
					terrain_player_sight_device[x][y] -= height * (radius - distance) / radius + base_floor;
					if (terrain_player_sight_device[x][y] > max_height) {
						terrain_player_sight_device[x][y] = max_height;
					}
					else if (terrain_player_sight_device[x][y] < base_floor) {
						terrain_player_sight_device[x][y] = base_floor;
					}
				}
			}
		}
	}
	else {
		//맵 밖의 경우 0
		terrain_player_sight_device[x][y] = 0;
	}
}

class Map
{
private:
	char** terrain_array_host = new char* [one_side_number];
	char** terrain_array_device;
	char* terrain_array_temp[one_side_number];

	char** terrain_player_sight_host = new char* [player_sight_size];

	int num_of_hills;
	int origin_num_of_hills;

public:
	Map()
	{
		//Make Random Hills Information===================================================
		clock_t t_0 = clock();

		HI* hill_location_host;
		HI* hill_location_device;
		hill_location_host = new HI[4000];
		hill_location_device;
		cudaMalloc((void**)&hill_location_device, 4000 * sizeof(HI));
		num_of_hills = make_hill_location(hill_location_host);
		origin_num_of_hills = num_of_hills;
		cudaMemcpy(hill_location_device, hill_location_host, num_of_hills * sizeof(HI), cudaMemcpyHostToDevice); //Memcpy to Device
		printf("Random Hill Info Complete\n");
		for (int i = 0; i < num_of_hills; i++) {
			cout << hill_location_host[i].x << ", " << hill_location_host[i].y << ", " << hill_location_host[i].height << ", " << hill_location_host[i].radius << endl;
		}


		//Terrain Memory Assignement===================================================
		clock_t t_1 = clock();
		for (int i = 0; i < one_side_number; i++) {
			terrain_array_host[i] = new char[one_side_number];
		}
		for (int i = 0; i < one_side_number; i++) {
			for (int j = 0; j < one_side_number; j++) {
				terrain_array_host[i][j] = 1;
			}
		}
		cudaMalloc((void**)&terrain_array_device, one_side_number * sizeof(char*));
		for (int i = 0; i < one_side_number; i++) {
			cudaMalloc((void**)&terrain_array_temp[i], one_side_number * sizeof(char));
		}
		cudaMemcpy(terrain_array_device, terrain_array_temp, one_side_number * sizeof(char*), cudaMemcpyHostToDevice);
		for (int i = 0; i < one_side_number; i++) {
			cudaMemcpy(terrain_array_temp[i], terrain_array_host[i], one_side_number * sizeof(char), cudaMemcpyHostToDevice);
		}


		//Terrain Memory Assignment For Player's Sight===================================================
		clock_t t_2 = clock();
		for (int i = 0; i < player_sight_size; i++) {
			terrain_player_sight_host[i] = new char[player_sight_size];
		}
		for (int i = 0; i < player_sight_size; i++) {
			for (int j = 0; j < player_sight_size; j++) {
				terrain_player_sight_host[i][j] = 0;
			}
		}


		//Make Hills===================================================
		clock_t t_3 = clock();
		dim3 grid(one_side_number / 32, one_side_number / 32, 1);
		dim3 block(32, 32, 1);
		make_hills_cuda << <grid, block >> > (terrain_array_device, hill_location_device, num_of_hills);
		//make_hills_cuda << <one_side_number, num_of_hills >> > (terrain_array_device, hill_location_device);
		for (int i = 0; i < one_side_number; i++) {
			cudaMemcpy(terrain_array_host[i], terrain_array_temp[i], one_side_number * sizeof(char), cudaMemcpyDeviceToHost);
		}
		printf("Terrain Generation Complete\n");
		clock_t  t_4 = clock();

		cout << "Terrain size : " << one_side_number << " * " << one_side_number << endl;
		cout << "Terrain Array Size : " << one_side_number * one_side_number * sizeof(char) << " Bytes" << endl;
		cout << "Make Random Hills Information : " << (double)(t_1 - t_0) / CLOCKS_PER_SEC << " sec" << endl;
		cout << "Terrain Memory Assignement : " << (double)(t_2 - t_1) / CLOCKS_PER_SEC << " sec" << endl;
		cout << "Terrain Memory Assignment For Player's Sight : " << (double)(t_3 - t_2) / CLOCKS_PER_SEC << " sec" << endl;
		cout << "Make Hills : " << (double)(t_4 - t_3) / CLOCKS_PER_SEC << " sec" << endl;
		cout << endl;

	}

	~Map()
	{
		for (int i = 0; i < one_side_number; i++) {
			delete[] terrain_array_host[i];
		}
		for (int i = 0; i < player_sight_size; i++) {
			delete[] terrain_player_sight_host[i];
		}
		delete[] terrain_array_host;
		delete[] terrain_player_sight_host;
		cudaFree(terrain_array_temp);
		cudaFree(terrain_array_device);
	}
	
	CC get_highest_lowest()
	{
		clock_t t_0 = clock();
		char highest = terrain_array_host[0][0];
		char lowest = terrain_array_host[0][0];
		for (int i = 0; i < one_side_number; i++) {
			for (int j = 0; j < one_side_number; j++) {
				if (terrain_array_host[i][j] > highest) {
					highest = terrain_array_host[i][j];
				}
				else if (terrain_array_host[i][j] < lowest) {
					lowest = terrain_array_host[i][j];
				}
			}
		}
		CC value{ highest, lowest };
		clock_t t_1 = clock();
		cout << "Get Highest : " << (double)(t_1 - t_0) / CLOCKS_PER_SEC << " sec" << endl;
		return value;
	}

	void terrain_change()
	{
		clock_t t_0 = clock();
		CC hi_low = get_highest_lowest();
		cout << hi_low.x << " " << hi_low.y << endl;
		dim3 grid(one_side_number / 32, one_side_number / 32, 1);
		dim3 block(32, 32, 1);
		for (int height = hi_low.x; height > hi_low.y; height--) {
			for (int j = 0; j < 1; j++) {
				terrain_change_cuda << <grid, block >> > (terrain_array_device, height);
			}
		}
		
		for (int i = 0; i < one_side_number; i++) {
			cudaMemcpy(terrain_array_host[i], terrain_array_temp[i], one_side_number * sizeof(char), cudaMemcpyDeviceToHost);
		}
		
		clock_t t_1 = clock();
		cout << "Terrain Flatten : " << (double)(t_1 - t_0) / CLOCKS_PER_SEC << " sec" << endl;
	}

	void wind_blow(int wind_angle, int wind_speed)
	{
		clock_t t_0 = clock();
		//1m 이동이면 블럭 4칸이니, 풍향을 90도 단위로 주되, 여러 방향을 여러번 적용시켜서 원하는 풍향에 맞춘다.
		//만약 3번 왼쪽으로 주고 1번 위로 주면, 이동거리는 1m이고, 풍향은 25도가 된다.
		//2차원 배열이라, 정확하진 않지만 근삿값은 됨.
		
		FF wind_direction = { cos(wind_angle * PI / 180), sin(wind_angle * PI / 180) };
		if (abs(wind_direction.x) < FLT_EPSILON) {
			wind_direction.x = 0;
		}
		if (abs(wind_direction.y) < FLT_EPSILON) {
			wind_direction.y = 0;
		}
		
		dim3 grid(one_side_number / 32, one_side_number / 32, 1);
		dim3 block(32, 32, 1);
		wind_blow_cuda << <grid, block >> > (terrain_array_device, wind_direction, wind_speed);
		for (int i = 0; i < one_side_number; i++) {
			cudaMemcpy(terrain_array_host[i], terrain_array_temp[i], one_side_number * sizeof(char), cudaMemcpyDeviceToHost);
		}
		clock_t t_1 = clock();
		cout << "Wind Blow : " << (double)(t_1 - t_0) / CLOCKS_PER_SEC << " sec" << endl;
	}
	
	void add_all()
	{
		clock_t t_0 = clock();
		unsigned int all = 0;
		for (int i = 0; i < one_side_number; i++) {
			for (int j = 0; j < one_side_number; j++) {
				all += terrain_array_host[i][j];
			}
		}

		clock_t t_1 = clock();
		cout << all << endl;
		cout << "Terrain Add All : " << (double)(t_1 - t_0) / CLOCKS_PER_SEC << " sec" << endl;
	}
	
	void show_array(char** terrain_array_host, int size)
	{
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				printf("%d ", terrain_array_host[x][y]);
			}
			printf("\n");
		}
	}

	void copy_for_player_map(II player_location)
	{
		clock_t start_t, end_t;
		start_t = clock();
		for (int i = 0; i < player_sight_size; i++) {
			for (int j = 0; j < player_sight_size; j++) {
				if (player_location.x - player_sight_size / 2 + i < 0 || player_location.x - player_sight_size / 2 + i >= one_side_number || player_location.y - player_sight_size / 2 + j < 0 || player_location.y - player_sight_size / 2 + j >= one_side_number)
					terrain_player_sight_host[i][j] = 0;
				else
					terrain_player_sight_host[i][j] = terrain_array_host[player_location.x - player_sight_size / 2 + i][player_location.y - player_sight_size / 2 + j];
			}
		}
		end_t = clock();
		cout << "copy_for_player_map : " << double(end_t - start_t) / CLOCKS_PER_SEC << endl;
	}
	
	void update_player_sight()
	{
		//Terrain move & Player Sight Update===================================================
		//for (int i = 0; i < 1; i++) {
		//	clock_t t_1 = clock();

		//	//Terrain Move
		//	wind_decide(wind_speed, wind_angle);

		//	FF wind_direction = { cos(wind_angle * PI / 180), sin(wind_angle * PI / 180) };
		//	if (abs(wind_direction.x) < FLT_EPSILON) {
		//		wind_direction.x = 0;
		//	}
		//	if (abs(wind_direction.y) < FLT_EPSILON) {
		//		wind_direction.y = 0;
		//	}

		//	move_terrain(hill_location_host, num_of_hills, wind_direction, wind_speed);
		//	if (num_of_hills < origin_num_of_hills) {
		//		make_new_hills(hill_location_host, num_of_hills, origin_num_of_hills, wind_direction, wind_speed);
		//	}

		//	cudaMemcpy(hill_location_device, hill_location_host, num_of_hills * sizeof(HI), cudaMemcpyHostToDevice); //Memcpy to Device

		//	//Player Sight Update
		//	//player_location.x += 20;
		//	//player_location.y += 20;
		//	//thread must be 1024 for efficiency
		//	player_terrain_update_cuda << <player_sight_size, player_sight_size >> > (terrain_player_sight_device, hill_location_device, num_of_hills, player_location, wind_direction, wind_speed);
		//	for (int i = 0; i < player_sight_size; i++) {
		//		cudaMemcpy(terrain_player_sight_host[i], terrain_player_sight_temp[i], player_sight_size * sizeof(char), cudaMemcpyDeviceToHost);
		//	}
		//	clock_t t_2 = clock();
		//	cout << "Player Sight Update Time : " << (double)(t_2 - t_1) / CLOCKS_PER_SEC << " Seconds" << endl;
		//	//show_array(terrain_player_sight_host, player_sight_size);
		//	cout << "==============================" << endl;
		//}
	}
	
	void get_device_info()
	{
		cudaDeviceProp  prop;

		int count;
		cudaGetDeviceCount(&count);

		for (int i = 0; i < count; i++) {
			cudaGetDeviceProperties(&prop, i);
			printf("   --- General Information for device %d ---\n", i);
			printf("Name:  %s\n", prop.name);
			printf("Compute capability:  %d.%d\n", prop.major, prop.minor);
			printf("Clock rate:  %d\n", prop.clockRate);
			printf("Device copy overlap:  ");
			if (prop.deviceOverlap)
				printf("Enabled\n");
			else
				printf("Disabled\n");
			printf("Kernel execution timeout :  ");
			if (prop.kernelExecTimeoutEnabled)
				printf("Enabled\n");
			else
				printf("Disabled\n");
			printf("\n");

			printf("   --- Memory Information for device %d ---\n", i);
			printf("Total global mem:  %ld\n", prop.totalGlobalMem);
			printf("Total constant Mem:  %ld\n", prop.totalConstMem);
			printf("Max mem pitch:  %ld\n", prop.memPitch);
			printf("Texture Alignment:  %ld\n", prop.textureAlignment);
			printf("\n");

			printf("   --- MP Information for device %d ---\n", i);
			printf("Multiprocessor count:  %d\n", prop.multiProcessorCount);
			printf("Shared mem per mp:  %ld\n", prop.sharedMemPerBlock);
			printf("Registers per mp:  %d\n", prop.regsPerBlock);
			printf("Threads in warp:  %d\n", prop.warpSize);
			printf("Max threads per block:  %d\n", prop.maxThreadsPerBlock);
			printf("Max thread dimensions:  (%d, %d, %d)\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
			printf("Max grid dimensions:  (%d, %d, %d)\n", prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]);
			printf("\n");
		}
	}
	
	template <typename T>
	int delete_array(T* array, int i, int size)
	{
		for (int j = i; j < size; j++) {
			array[j] = array[j + 1];
		}
		size -= 1;
		return size;
	}

	int make_hill_location(HI* hill_location_host)
	{
		num_of_hills = number_of_hills_uid(dre);
		cout << "expected num of hills: " << num_of_hills << endl;

		for (int i = 0; i < num_of_hills; i++) {
			hill_location_host[i].x = hills_location(dre);
			hill_location_host[i].y = hills_location(dre);
			hill_location_host[i].radius = hill_size_uid(dre);
			hill_location_host[i].height = height_uid(dre);
		}
		//sort(&hill_location_host[0], &hill_location_host[num_of_hills], [](const HI& a, const HI& b) { return a.y < b.y; });
		for (int a = 0; a < num_of_hills; a++) {
			for (int b = 0; b < num_of_hills; b++) {
				if (a != b) {
					if (pow(hill_location_host[a].x - hill_location_host[b].x, 2) - pow(hill_location_host[a].radius + hill_location_host[b].radius, 2) < 0) {
						if (pow(hill_location_host[a].y - hill_location_host[b].y, 2) - pow(hill_location_host[a].radius + hill_location_host[b].radius, 2) < 0) {
							num_of_hills = delete_array(hill_location_host, b, num_of_hills);
							b--;
						}
					}
				}
			}
		}
		//for (int i = 0; i < num_of_hills; i++) {
		//	if (hill_location_host[i].x - hill_location_host[i].radius < 0) {
		//		num_of_hills = delete_array(hill_location_host, i, num_of_hills);
		//		--i;
		//		continue;
		//	}
		//	else if (hill_location_host[i].x + hill_location_host[i].radius >= one_side_number) {
		//		num_of_hills = delete_array (hill_location_host, i, num_of_hills);
		//		--i;
		//		continue;
		//	}
		//	else if (hill_location_host[i].y - hill_location_host[i].radius < 0) {
		//		num_of_hills = delete_array(hill_location_host, i, num_of_hills);
		//		--i;
		//		continue;
		//	}
		//	else if (hill_location_host[i].y + hill_location_host[i].radius >= one_side_number) {
		//		num_of_hills = delete_array(hill_location_host, i, num_of_hills);
		//		--i;
		//		continue;
		//	}
		//}
		cout << "real num of hills: " << num_of_hills << endl;
		return num_of_hills;
	}

	void make_new_hills(HI* hill_location_host, int& num_of_hills, int origin_num_of_hills, FF wind_direction, int wind_speed)
	{
		cout << "Wind info: " << wind_direction.x << " " << wind_direction.y << endl;
		if (wind_speed) {
			hill_location_host[num_of_hills].radius = hill_size_uid(dre);
			hill_location_host[num_of_hills].height = height_uid(dre);

			hill_location_host[num_of_hills].x = hills_location(dre);
			hill_location_host[num_of_hills].y = hills_location(dre);

			while (1) {
				hill_location_host[num_of_hills].x -= wind_direction.x * wind_speed;
				hill_location_host[num_of_hills].y -= wind_direction.y * wind_speed;
				if (hill_location_host[num_of_hills].x - hill_location_host[num_of_hills].radius > one_side_number) {
					break;
				}
				if (hill_location_host[num_of_hills].x + hill_location_host[num_of_hills].radius < 0) {
					break;
				}
				if (hill_location_host[num_of_hills].y - hill_location_host[num_of_hills].radius > one_side_number) {
					break;
				}
				if (hill_location_host[num_of_hills].y + hill_location_host[num_of_hills].radius < 0) {
					break;
				}
			}

			int collide_iter{};
			for (int a = 0; a < num_of_hills; a++) {
				if (collide_iter > 10) {	//무한루프 빠질 가능성으로 인해 횟수 제한
					return;
				}
				//cout << "대상: "<<a << " " << hill_location_host[a].x << " " << hill_location_host[a].y << " " << hill_location_host[a].radius << " " << hill_location_host[a].height << endl;
				//cout << " 나: "<< num_of_hills<< " " << hill_location_host[num_of_hills].x << " " << hill_location_host[num_of_hills ].y << " " << hill_location_host[num_of_hills ].radius << " " << hill_location_host[num_of_hills].height << endl << endl;

				if (pow(hill_location_host[a].x - hill_location_host[num_of_hills].x, 2) - pow(hill_location_host[a].radius + hill_location_host[num_of_hills].radius, 2) <= 0) {
					if (pow(hill_location_host[a].y - hill_location_host[num_of_hills].y, 2) - pow(hill_location_host[a].radius + hill_location_host[num_of_hills].radius, 2) <= 0) {

						hill_location_host[num_of_hills].radius = hill_size_uid(dre);
						hill_location_host[num_of_hills].height = height_uid(dre);

						hill_location_host[num_of_hills].x = hills_location(dre);
						hill_location_host[num_of_hills].y = hills_location(dre);

						while (1) {
							hill_location_host[num_of_hills].x -= wind_direction.x * wind_speed;
							hill_location_host[num_of_hills].y -= wind_direction.y * wind_speed;
							if (hill_location_host[num_of_hills].x - hill_location_host[num_of_hills].radius > one_side_number) {
								break;
							}
							if (hill_location_host[num_of_hills].x + hill_location_host[num_of_hills].radius < 0) {
								break;
							}
							if (hill_location_host[num_of_hills].y - hill_location_host[num_of_hills].radius > one_side_number) {
								break;
							}
							if (hill_location_host[num_of_hills].y + hill_location_host[num_of_hills].radius < 0) {
								break;
							}
						}
						cout << "충돌로 인해 바꿈" << endl;
						a = -1;
						collide_iter++;
					}
				}

			}
			cout << " 최종: " << num_of_hills << " " << hill_location_host[num_of_hills].x << " " << hill_location_host[num_of_hills].y << " " << hill_location_host[num_of_hills].radius << " " << hill_location_host[num_of_hills].height << endl << endl;
			num_of_hills++;
		}
	}

	void move_terrain(HI* hill_location_host, int& num_of_hills, FF wind_direction, int wind_speed)
	{
		if (wind_speed) {
			int wind_move_x = wind_speed * wind_direction.x;
			int wind_move_y = wind_speed * wind_direction.y;
			cout << "Wind == " << "X: " << wind_move_x << " " << "Y: " << wind_move_y << endl;
			for (int i = 0; i < num_of_hills; i++) {
				hill_location_host[i].x += wind_move_x;
				hill_location_host[i].y += wind_move_y;

				if (hill_location_host[i].x - hill_location_host[i].radius > one_side_number) {
					num_of_hills = delete_array(hill_location_host, i, num_of_hills);
					--i;
					continue;
				}
				if (hill_location_host[i].x + hill_location_host[i].radius < 0) {
					num_of_hills = delete_array(hill_location_host, i, num_of_hills);
					--i;
					continue;
				}
				if (hill_location_host[i].y - hill_location_host[i].radius > one_side_number) {
					num_of_hills = delete_array(hill_location_host, i, num_of_hills);
					--i;
					continue;
				}
				if (hill_location_host[i].y + hill_location_host[i].radius < 0) {
					num_of_hills = delete_array(hill_location_host, i, num_of_hills);
					--i;
					continue;
				}
			}
			/*for (int i = 0; i < num_of_hills; i++) {
				cout << hill_location_host[i].x << " " << hill_location_host[i].y << endl;
			}
			cout << endl;*/
		}
	}

	void wind_decide(int& wind_speed, int& wind_angle)
	{
		//wind speed 0-50
		//wind angle 0-360
		wind_speed = 50;// wind_speed_uid(dre);
		//wind_angle = 90; // wind_angle_uid(dre);
		wind_angle += 10;
		cout << wind_speed << " " << wind_angle << endl;
		//풍향을 언제마다 한번 업데이트 할 것인지, 풍속은 언제마다 한번 업데이트 할 것인지 회의를 통해 결정하자
	}
	
	char** get_map() {
		return terrain_array_host;
	}

	char** get_player_sight_map() {
		return terrain_player_sight_host;
	}
};
