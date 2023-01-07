#include <iostream>
#include <stdio.h>
#include <vector>
#include <random>
#include <windows.h>
#include <time.h>
#include <fstream>
#include <cuda_runtime.h>
#include <curand.h>
#include <curand_kernel.h>
#include <cooperative_groups.h>
#include "device_launch_parameters.h"
using namespace std;

const unsigned int one_side_number = 1000;	//39936
const int player_sight_size = 50;	//1024 넘으면 안됨

const int max_height = 8;
const int base_floor = 1;

typedef struct two_int {
	int x;
	int y;
} TI;

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
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			printf("%d ", terrain_array_host[x][y]);
		}
		printf("\n");
	}
}

__global__
void make_hills_cuda(char** terrain_array_device, HI* hill_location_device)
{
	//int id = threadIdx.x + blockIdx.x * blockDim.x;
	int my_hill = threadIdx.x;
	int my_y = blockIdx.x;

	int hill_location_x = hill_location_device[my_hill].x;
	int hill_location_y = hill_location_device[my_hill].y;
	int radius = hill_location_device[my_hill].radius;
	int height = hill_location_device[my_hill].height;
	int distance{};

	//printf("threadId.x: %d, blockIdx.x: %d, blockDim.x: %d, id: %d\n", threadIdx.x, blockIdx.x, blockDim.x, id);
	//printf("Hill %d : (%d, %d) / %d / %d\n", my_hill, hill_location_x, hill_location_y, radius, height);

	for (int x = hill_location_x - radius; x <= hill_location_x + radius; ++x) {
		distance = sqrt(((pow(x - hill_location_x, 2)) + (pow(my_y - hill_location_y, 2))));
		if (distance <= radius) {
			terrain_array_device[x][my_y] += (height - 1) * (radius - distance) / radius + 1;
			if (terrain_array_device[x][my_y] > max_height) {
				terrain_array_device[x][my_y] = max_height;
			}
			else if (terrain_array_device[x][my_y] < 0) {
				terrain_array_device[x][my_y] = 0;
			}
		}
	}
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
	sort(&hill_location_host[0], &hill_location_host[num_of_hills], [](const HI& a, const HI& b) { return a.y < b.y; });
	for (int a = 0; a < num_of_hills; a++) {
		for (int b = 0; b < num_of_hills; b++) {
			if (a != b) {
				if (pow(hill_location_host[a].x - hill_location_host[b].x, 2) - pow(hill_location_host[a].radius + hill_location_host[b].radius, 2) < 0) {
					if (pow(hill_location_host[a].y - hill_location_host[b].y, 2) - pow(hill_location_host[a].radius + hill_location_host[b].radius, 2) < 0) {
						for (int i = b; i < num_of_hills; i++) {
							hill_location_host[i] = hill_location_host[i + 1];
						}
						b -= 1;
						num_of_hills -= 1;
					}
				}
			}
		}
	}
	for (int i = 0; i < num_of_hills; i++) {
		if (hill_location_host[i].x - hill_location_host[i].radius < 0) {
			for (int j = i; j < num_of_hills; j++) {
				hill_location_host[j] = hill_location_host[j + 1];
			}
			i -= 1;
			num_of_hills -= 1;
		}
		else if (hill_location_host[i].x + hill_location_host[i].radius >= one_side_number) {
			for (int j = i; j < num_of_hills; j++) {
				hill_location_host[j] = hill_location_host[j + 1];
			}
			i -= 1;
			num_of_hills -= 1;
		}
		else if (hill_location_host[i].y - hill_location_host[i].radius < 0) {
			for (int j = i; j < num_of_hills; j++) {
				hill_location_host[j] = hill_location_host[j + 1];
			}
			i -= 1;
			num_of_hills -= 1;
		}
		else if (hill_location_host[i].y + hill_location_host[i].radius >= one_side_number) {
			for (int j = i; j < num_of_hills; j++) {
				hill_location_host[j] = hill_location_host[j + 1];
			}
			i -= 1;
			num_of_hills -= 1;
		}
	}
	cout << "real num of hills: " << num_of_hills << endl;
	return num_of_hills;
}

__global__
void player_terrain_update_cuda(char** terrain_array_device, char** terrain_player_sight_device, TI player_location, int player_sight_size)
{
	//int id = threadIdx.x + blockIdx.x * blockDim.x;
	int x = threadIdx.x;
	int y = blockIdx.x;
	//printf("threadId.x: %d, blockIdx.x: %d, blockDim.x: %d\n", threadIdx.x, blockIdx.x, blockDim.x);

	if (x + player_location.x > 0 && x + player_location.x < one_side_number && y + player_location.y > 0 && y + player_location.y < one_side_number) {
		terrain_player_sight_device[x][y] = terrain_array_device[x + player_location.x][y + player_location.y];
	}
	else {
		terrain_player_sight_device[x][y] = 0;
	}
}

int main()
{
	//ToDo
	//마을 위치 랜덤 생성, 마을 위치는 높이 0인데 주변 언덕이 점차적인 높이로함, 언덕 움직이기
	// 언덕을 움직이는데 효울적인 방법: 플레이어가 보는 시야만 업데이트를 하게끔 해야함 
	// hill_location 업데이트는 항상 하고, cuda로 플레이어가 보는 시점만 terrain_array를 업데이트 해야함
	// 플레이어가 보는 시점을 1000*1000이라 가정하고, 그 부분의 terrain_array를 업데이트 해야함
	// 플레이어 수에 맞게 플레이어 시점 2차원 배열 terrain_array_for_player를 동적할당하여 생성해야함
	// 
	//
	
	//get_device_info();

	//Make Random Hills Information===================================================
	clock_t random_hills_info_start_time = clock();
	HI* hill_location_host = new HI[10000];
	HI* hill_location_device;
	cudaMalloc((void**)&hill_location_device, 10000 * sizeof(HI));
	int num_of_hills = make_hill_location(hill_location_host);
	cudaMemcpy(hill_location_device, hill_location_host, num_of_hills * sizeof(HI), cudaMemcpyHostToDevice); //Memcpy to Device
	printf("Random Hill Info Complete\n");
	
		
	//Terrain Memory Assignement===================================================
	clock_t memory_assign_start_time = clock();
	char** terrain_array_host = new char* [one_side_number];	// 2D array for host
	for (int i = 0; i < one_side_number; i++) {
		terrain_array_host[i] = new char[one_side_number];
	}
	for (int i = 0; i < one_side_number; i++) {
		for (int j = 0; j < one_side_number; j++) {
			terrain_array_host[i][j] = 1;
		}
	}
	char** terrain_array_device;					// 2D array for device
	char* terrain_array_temp[one_side_number];		// 1D array temp
	cudaMalloc((void**)&terrain_array_device, one_side_number * sizeof(char*));
	for (int i = 0; i < one_side_number; i++) {
		cudaMalloc((void**)&terrain_array_temp[i], one_side_number * sizeof(char));
	}
	cudaMemcpy(terrain_array_device, terrain_array_temp, one_side_number* sizeof(char*), cudaMemcpyHostToDevice);
	for (int i = 0; i < one_side_number; i++) {
		cudaMemcpy(terrain_array_temp[i], terrain_array_host[i], one_side_number * sizeof(char), cudaMemcpyHostToDevice);
	}
	
	
	//Make Hills===================================================
	clock_t terrain_generate_start_time = clock();
	make_hills_cuda << <one_side_number, num_of_hills >> > (terrain_array_device, hill_location_device);
	for (int i = 0; i < one_side_number; i++) {	
		cudaMemcpy(terrain_array_host[i], terrain_array_temp[i], one_side_number * sizeof(char), cudaMemcpyDeviceToHost);
	}
	printf("Terrain Generation Complete\n");
	clock_t  terrain_generate_end_time = clock();

	//show_array(terrain_array_host, one_side_number);
	cout << "Terrain size : " << one_side_number << " * " << one_side_number << endl;
	cout << "Terrain Array Size : " << one_side_number * one_side_number * sizeof(char) << " Bytes" << endl;
	cout << "Random Hills Info Time : " << (double)(memory_assign_start_time - random_hills_info_start_time) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "Memory Assign Time : " << (double)(terrain_generate_start_time - memory_assign_start_time) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "Terrain Generate Time : " << (double)(terrain_generate_end_time - terrain_generate_start_time) / CLOCKS_PER_SEC << " Seconds" << endl;
	cout << "Total Time : " << (double)(terrain_generate_end_time - random_hills_info_start_time) / CLOCKS_PER_SEC << " Seconds" << endl;

	
	//Terrain Memory Assignment For Player's Sight===================================================
	char** terrain_player_sight_host = new char* [player_sight_size];	// 2D array for host
	for (int i = 0; i < player_sight_size; i++) {
		terrain_player_sight_host[i] = new char[player_sight_size];
	}
	for (int i = 0; i < player_sight_size; i++) {
		for (int j = 0; j < player_sight_size; j++) {
			terrain_player_sight_host[i][j] = 0;
		}
	}
	char** terrain_player_sight_device;					// 2D array for device
	char* terrain_player_sight_temp[player_sight_size];		// 1D array temp
	cudaMalloc((void**)&terrain_player_sight_device, player_sight_size * sizeof(char*));
	for (int i = 0; i < player_sight_size; i++) {
		cudaMalloc((void**)&terrain_player_sight_temp[i], player_sight_size * sizeof(char));
	}
	cudaMemcpy(terrain_player_sight_device, terrain_player_sight_temp, player_sight_size * sizeof(char*), cudaMemcpyHostToDevice);
	for (int i = 0; i < player_sight_size; i++) {
		cudaMemcpy(terrain_player_sight_temp[i], terrain_player_sight_host[i], player_sight_size * sizeof(char), cudaMemcpyHostToDevice);
	}

	
	//Player Sight Update===================================================
	TI player_location = { 1,500 };

	for (int i = 0; i < 10; i++) {
		clock_t player_sight_update_start_time = clock();
		player_location.x += 50;
		player_location.y += 50;
		//thread 1024 넘으면 문제 안생기나?
		player_terrain_update_cuda << <player_sight_size, player_sight_size >> > (terrain_array_device, terrain_player_sight_device, player_location, player_sight_size);
		for (int i = 0; i < player_sight_size; i++) {
			cudaMemcpy(terrain_player_sight_host[i], terrain_player_sight_temp[i], player_sight_size * sizeof(char), cudaMemcpyDeviceToHost);
		}
		clock_t player_sight_update_end_time = clock();
		cout << "Player Sight Update Time : " << (double)(player_sight_update_end_time - player_sight_update_start_time) / CLOCKS_PER_SEC << " Seconds" << endl;
		show_array(terrain_player_sight_host, player_sight_size);
		cout << "==============================" << endl;
	}
	
	
	//File Save===================================================
	/*ofstream out{"terrain.txt"};

	for (int i = 0; i < one_side_number; i++) {
		for (int j = 0; j < one_side_number; j++) {
			out << terrain_array_host[i][j] << " ";
		}
		out << endl;
	}
	cout << "File Write Complete" << endl;*/
	

	//Free Memory===================================================
	delete hill_location_host;
	cudaFree(hill_location_device);
	
	for (int i = 0; i < one_side_number; i++) {
		delete[] terrain_array_host[i];
	}
	delete[] terrain_array_host;
	cudaFree(terrain_array_device);
	for (int i = 0; i < one_side_number; i++) {
		cudaFree(terrain_array_temp[i]);
	}
}
