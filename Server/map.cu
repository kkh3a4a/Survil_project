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

const int one_side_number = 40000;	//39936
const int player_sight_size = 1000;	//1024 넘으면 안됨

const int max_height = 8;
const int base_floor = 1;

typedef struct two_int {
	int x;
	int y;
} II;

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

random_device rd;
default_random_engine dre(rd());
uniform_int_distribution <int>hills_location(0, one_side_number);
uniform_int_distribution <int>number_of_hills_uid(one_side_number / 10, one_side_number / 10);
uniform_int_distribution <int>hill_size_uid(one_side_number / 20, one_side_number / 10);
uniform_int_distribution <int>height_uid(5, max_height);

uniform_int_distribution <int>wind_speed_uid(0, 50);
uniform_int_distribution <int>wind_angle_uid(0, 360);


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

void show_array(char** terrain_array_host, int size)
{
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			printf("%d ", terrain_array_host[x][y]);
		}
		printf("\n");
	}
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
	int num_of_hills = number_of_hills_uid(dre);
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
//
//int main()
//{
//	//get_device_info();
//
//
//	//ToDo
//	//마을 위치 랜덤 생성, 마을 위치는 높이 0인데 주변 언덕이 점차적인 높이로함, 언덕 움직이기
//	// 언덕을 움직이는데 효울적인 방법: 플레이어가 보는 시야만 업데이트를 하게끔 해야함 
//	// hill_location 업데이트는 항상 하고, cuda로 플레이어가 보는 시점만 terrain_array를 업데이트 해야함
//	// 플레이어가 보는 시점을 1000*1000이라 가정하고, 그 부분의 terrain_array를 업데이트 해야함
//	// 플레이어 수에 맞게 플레이어 시점 2차원 배열 terrain_array_for_player를 동적할당하여 생성해야함
//	// 
//	// 플레이어에게 맵 정보를 보내줘야 할 시기: 게임 시작, 지형이 변경될 때, 카메라가 움직일 때
//	// 문제: 지형이 움직일때 terrain_array전체를 수정하고, 플레이어에게 terrain_array에 복사만 해서 일부분을 보여줄지,
//	// hill_location을 수정하고, 플레이어에게 보여줄때, terrain_array를 아예 그릴지. ====이게 나을듯...40000*40000 만드는건 뻘짓이였다..
//	//
//	// 지형이 움직이면, 움직이는 방향의 반대 부분은 비게 된다. 언덕이 없다던가 등등...
//	// 지형이 한 방향으로 움직이면 생성해줘야함
//	// 그리고 40000 * 40000 밖으로 나간 언덕은  hill_location 배열에서 지워줘야함.
//	// 
//	// 언덕이 생성될때 언덕의 가파른 정도를 생각해보는 것도 좋을듯 하다
//	// 
//	// 사막의 모래언덕 모양을 위에서 보게 된다면 상현달~초승달의 모양을 띄게 되는것같다
//	// 바람의 세기와 방향에 따라서 달 모양이 향하는 방향을 바꿔줘야한다.
//	// 바람이 한번도 불지 않았다면 원 모양을 띈다는 전제하에 바람이 불면, 바람이 온 방향쪽을 깎아주는 모양을 만들자
//	// 바람이 서쪽에서 분다면 언덕의 왼쪽 부분을 깎고 언덕이 오른쪽으로 이동하게끔 한다,
//	// 그 후 바람이 동쪽에서 분다면 한칸씩 왼쪽은 차게되고, 오른쪽은 깎이는 형식으로 만들자.
//	// 바람의 방향이 끼치는 영향: 언덕이 깎였던 부분이 채워지는 방향, 언덕이 깎이는 방향
//	// 바람의 세기가 끼치는 영향: 지형이 이동하는 속도, 언덕이 깎이는 속도
//	// 
//	// //현재 언덕이 깎이는 모습을 제대로 보이게 하려면 풍속과 풍향을 확확 바꾸면 안된다.
//	// 최대 풍속이 50이면 바람이 안부는 상태에서 10, 20, 30, 40, 50 이렇게 점점 늘려서 클라한테 보내줘야 한다
//	// 아니면 언덕의 모양이 한번에 확 변한다
//	// 
//	// 블럭의 가로 세로 높이 비율을 1:1:0.2로 하면 어떨까 생각이 듦.
//	// 만약 1:1:1이라면 붙어있는 블럭의 경사는 0, 45도 밖에 없음
//	// 
//	// 지형이 바람에 의해 이동하면, 이동한 방향의 반대 방향에 언덕이 새로 생성되어야 한다.
//	// 처음에 생성된 언덕의 개수를 기억하고, 언덕이 사라지게 되면 그만큼 새로 생성되게끔 하자.
//	// 왼쪽으로 바람이 불어 움직이고 있다면 오른쪽에서 생성, 아래로 바람이 분다면 위에서 생성 하게끔.
//	// 언덕이 붙어서 생성되지 않게끔 해야한다. 추가를 할때 hill_location 배열을 확인해서 충돌검사를 하자.
//	// 
//	// 바람이 부는 알고리즘을 생성하자, 풍속과 풍향은 점차적으로 바뀌어야한다.
//	// 
//	// 
//	// 
//	//
//
//
//	//Make Random Hills Information===================================================
//	HI* hill_location_host = new HI[4000];
//	HI* hill_location_device;
//	cudaMalloc((void**)&hill_location_device, 4000 * sizeof(HI));
//	int num_of_hills = make_hill_location(hill_location_host);
//	int origin_num_of_hills = num_of_hills;
//	cudaMemcpy(hill_location_device, hill_location_host, num_of_hills * sizeof(HI), cudaMemcpyHostToDevice); //Memcpy to Device
//	printf("Random Hill Info Complete\n");
//
//
//	//Terrain Memory Assignment For Player's Sight===================================================
//	char** terrain_player_sight_host = new char* [player_sight_size];	// 2D array for host
//	for (int i = 0; i < player_sight_size; i++) {
//		terrain_player_sight_host[i] = new char[player_sight_size];
//	}
//	for (int i = 0; i < player_sight_size; i++) {
//		for (int j = 0; j < player_sight_size; j++) {
//			terrain_player_sight_host[i][j] = 0;
//		}
//	}
//	char** terrain_player_sight_device;						// 2D array for device
//	char* terrain_player_sight_temp[player_sight_size];		// 1D array temp
//	cudaMalloc((void**)&terrain_player_sight_device, player_sight_size * sizeof(char*));
//	for (int i = 0; i < player_sight_size; i++) {
//		cudaMalloc((void**)&terrain_player_sight_temp[i], player_sight_size * sizeof(char));
//	}
//	cudaMemcpy(terrain_player_sight_device, terrain_player_sight_temp, player_sight_size * sizeof(char*), cudaMemcpyHostToDevice);
//	for (int i = 0; i < player_sight_size; i++) {
//		cudaMemcpy(terrain_player_sight_temp[i], terrain_player_sight_host[i], player_sight_size * sizeof(char), cudaMemcpyHostToDevice);
//	}
//
//
//	//Terrain move & Player Sight Update===================================================
//	TI player_location = { 0, 0 };		//이거 나중에 중심 기준으로 바꿔야함
//	int wind_angle = 270;		//각도
//	int wind_speed = 50;		//최대 풍속 50
//	for (int i = 0; i < 10; i++) {
//		clock_t t_1 = clock();
//
//		//Terrain Move
//		wind_decide(wind_speed, wind_angle);
//
//		TF wind_direction = { cos(wind_angle * PI / 180), sin(wind_angle * PI / 180) };
//		if (abs(wind_direction.x) < FLT_EPSILON) {
//			wind_direction.x = 0;
//		}
//		if (abs(wind_direction.y) < FLT_EPSILON) {
//			wind_direction.y = 0;
//		}
//
//		move_terrain(hill_location_host, num_of_hills, wind_direction, wind_speed);
//		if (num_of_hills < origin_num_of_hills) {
//			make_new_hills(hill_location_host, num_of_hills, origin_num_of_hills, wind_direction, wind_speed);
//		}
//
//		cudaMemcpy(hill_location_device, hill_location_host, num_of_hills * sizeof(HI), cudaMemcpyHostToDevice); //Memcpy to Device
//
//		//Player Sight Update
//		//player_location.x += 20;
//		//player_location.y += 20;
//		//thread must be 1024 for efficiency
//		player_terrain_update_cuda << <player_sight_size, player_sight_size >> > (terrain_player_sight_device, hill_location_device, num_of_hills, player_location, wind_direction, wind_speed);
//		for (int i = 0; i < player_sight_size; i++) {
//			cudaMemcpy(terrain_player_sight_host[i], terrain_player_sight_temp[i], player_sight_size * sizeof(char), cudaMemcpyDeviceToHost);
//		}
//		clock_t t_2 = clock();
//		cout << "Player Sight Update Time : " << (double)(t_2 - t_1) / CLOCKS_PER_SEC << " Seconds" << endl;
//		//show_array(terrain_player_sight_host, player_sight_size);
//		cout << "==============================" << endl;
//	}
//
//
//
//	//File Save===================================================
//	/*ofstream out{"terrain.txt"};
//
//	for (int i = 0; i < one_side_number; i++) {
//		for (int j = 0; j < one_side_number; j++) {
//			out << terrain_array_host[i][j] << " ";
//		}
//		out << endl;
//	}
//	cout << "File Write Complete" << endl;*/
//
//
//	//Free Memory===================================================
//	delete[] hill_location_host;
//	cudaFree(hill_location_device);
//	for (int i = 0; i < player_sight_size; i++) {
//		delete[] terrain_player_sight_host[i];
//		cudaFree(terrain_player_sight_temp[i]);
//	}
//	delete[] terrain_player_sight_host;
//	cudaFree(terrain_player_sight_device);
//	cudaFree(terrain_player_sight_temp);
//}
