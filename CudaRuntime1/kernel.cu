//#include <iostream>
//#include <stdio.h>
//#include <random>
//#include <windows.h>
//#include <time.h>
//#include <cuda_runtime.h>
//#include <curand.h>
//#include <curand_kernel.h>
//using namespace std;
//
//const int one_side_number = 1000;
//const int max_height = 8;
//const int base_floor = 1;
//
//random_device rd;
// default_random_engine dre(rd());
//
//uniform_int_distribution <int>floor_uid(1, 2);
//
//uniform_int_distribution <int>hill_size_uid(6, 15);
//uniform_int_distribution <int>hills_location(0, one_side_number);
//uniform_int_distribution <int>number_of_hills_uid(5, 15);
// uniform_int_distribution <int>height_uid(5, max_height);
//
//char terrain_array_host[one_side_number * one_side_number]{};
//
//void show_array()
//{
//	for (int i = 0; i < one_side_number; ++i) {
//		for (int j = 0; j < one_side_number; ++j) {
//			printf("%d ", terrain_array_host[i * one_side_number + j]);
//		}
//		printf("\n");
//	}
//}
//
//void add_floor()
//{
//	for (int i = 0; i < one_side_number * one_side_number; i++){
//		terrain_array_host[i] += base_floor;
//	}
//}
//
//__global__ 
//void make_hills_cuda(int hill_location_x, int hill_location_y, int radius, int height, char* terrain_array_device_gpu)
//{
//	if ((int)terrain_array_device_gpu[hill_location_x * one_side_number + hill_location_y] + height > max_height) {
//		printf("%d, %d\n", terrain_array_device_gpu[hill_location_x * one_side_number + hill_location_y], height);
//		return;
//	}
//
//	for (int j = 0; j < one_side_number; j++) {
//		for (int k = 0; k < one_side_number; k++) {
//			int distance = sqrt(pow(j - hill_location_x, 2) + pow(k - hill_location_y, 2));
//			if (distance <= radius) {
//				terrain_array_device_gpu[j * one_side_number + k] += (height - 1) * (radius - distance) / radius + 1;
//				if ((int)terrain_array_device_gpu[j * one_side_number + k] > max_height) {
//					terrain_array_device_gpu[j * one_side_number + k] = (char)max_height;
//				}
//			}
//		}
//	}
//	/*for (int i = 0; i < one_side_number; ++i) {
//		for (int j = 0; j < one_side_number; ++j) {
//			printf("%d ", terrain_array_device_gpu[i * one_side_number + j]);
//		}
//		printf("\n");
//	}*/
//}
//
// void make_hills()
//{
//	char* terrain_array_device_gpu;
//	cudaMalloc(&terrain_array_device_gpu, one_side_number * one_side_number * sizeof(char));
//	cudaMemcpy(terrain_array_device_gpu, terrain_array_host, one_side_number * one_side_number * sizeof(char), cudaMemcpyHostToDevice);
//	 
//	for (int i = 0; i < number_of_hills_uid(dre); i++){
//		int height = height_uid(dre);
//		int radius = hill_size_uid(dre);
//		
//		int hill_location_x = hills_location(dre);
//		int hill_location_y = hills_location(dre);
//
//		make_hills_cuda <<<1, 1>>> ( hill_location_x, hill_location_y, radius, height, terrain_array_device_gpu);
//	}
//	cudaMemcpy(terrain_array_host, terrain_array_device_gpu, one_side_number * one_side_number * sizeof(char), cudaMemcpyDeviceToHost);
//}
//
//int main()
//{	
//	clock_t terrain_generate_start_time = clock();
//	make_hills();
//	clock_t  terrain_generate_end_time = clock();
//	clock_t memcpy_end_time = clock();
//	//add_floor();
//
//	show_array();
//	cout << "Terrain Generation Complete" << endl;
//	cout << "Terrain size : " << one_side_number << " * " << one_side_number << endl;
//	cout << "Terrain Array Size : " << sizeof(terrain_array_host) << " Bytes" << endl;
//	cout << "Terrain Generate Time : " << (double)(terrain_generate_end_time - terrain_generate_start_time) / CLOCKS_PER_SEC << " Seconds" << endl;
//	cout << "Memcpy Time : " << (double)(memcpy_end_time - terrain_generate_end_time) / CLOCKS_PER_SEC << " Seconds" << endl;
//}
//


//#include <iostream>
//#include <stdio.h>
//#include <random>
//#include <windows.h>
//#include <time.h>
//#include <cuda_runtime.h>
//#include <curand.h>
//#include <curand_kernel.h>
//using namespace std;
//
//const int one_side_number = 1000;
//const int max_height = 8;
//const int base_floor = 1;
//
//random_device rd;
//default_random_engine dre(rd());
//
//uniform_int_distribution <int>floor_uid(1, 2);
//
//uniform_int_distribution <int>hill_size_uid(one_side_number / 20, one_side_number / 10);
//uniform_int_distribution <int>hills_location(0, one_side_number);
//uniform_int_distribution <int>number_of_hills_uid(one_side_number / 10, one_side_number / 5);
//uniform_int_distribution <int>height_uid(5, max_height);
//
////uniform_int_distribution <int>hill_size_uid(20, 50);
////uniform_int_distribution <int>hills_location(0, one_side_number);
////uniform_int_distribution <int>number_of_hills_uid(10, 20);
////uniform_int_distribution <int>height_uid(5, max_height);
//
//__device__ char terrain_array_device[one_side_number * one_side_number]{};
//char terrain_array_host[one_side_number * one_side_number]{};
//
//void show_array()
//{
//	for (int i = 0; i < one_side_number; ++i) {
//		for (int j = 0; j < one_side_number; ++j) {
//			printf("%d ", terrain_array_host[i * one_side_number + j]);
//		}
//		printf("\n");
//	}
//}
//
//void add_floor()
//{
//	for (int i = 0; i < one_side_number * one_side_number; i++) {
//		terrain_array_host[i] += base_floor;
//	}
//}
//
////__device__ int getRand(curandState* s, int A, int B)
////{
////	float rand_int = curand_uniform(s);
////	rand_int = rand_int * (B - A) + A;
////	return rand_int;
////}
//
//__global__ void make_hills_cuda(int hill_location_x, int hill_location_y, int radius, int height, int i)
//{
//	/*int id = threadIdx.x + blockDim.x * blockDim.y;
//	unsigned int seed = id;
//	curandState s;
//	curand_init(seed, 0, 0, &s);
//	int rand_num = getRand(&s, 0, 10);
//	printf("%d\n", rand_num);*/
//
//	printf("%d 번째 언덕 생성 시작\n", i);
//	if (terrain_array_device[hill_location_x * one_side_number + hill_location_y] + height > max_height) {
//		printf("%d, %d\n", terrain_array_device[hill_location_x * one_side_number + hill_location_y], height);
//		return;
//	}
//
//	//for문 말고 멀티쓰레딩으로 하자
//	for (int j = 0; j < one_side_number; j++) {
//		for (int k = 0; k < one_side_number; k++) {
//			int distance = sqrt(pow(j - hill_location_x, 2) + pow(k - hill_location_y, 2));
//			if (distance <= radius) {
//				terrain_array_device[j * one_side_number + k] += (height - 1) * (radius - distance) / radius + 1;
//				if (terrain_array_device[j * one_side_number + k] > max_height) {
//					terrain_array_device[j * one_side_number + k] = max_height;
//				}
//			}
//		}
//	}
//}
//
//void make_hills()
//{
//	for (int i = 0; i < number_of_hills_uid(dre); i++) {
//		int height = height_uid(dre);
//		int radius = hill_size_uid(dre);
//
//		int hill_location_x = hills_location(dre);
//		int hill_location_y = hills_location(dre);
//
//		make_hills_cuda << <1, 1 >> > (hill_location_x, hill_location_y, radius, height, i);
//	}
//}
//
//int main()
//{
//	clock_t terrain_generate_start_time = clock();
//
//	make_hills();
//	clock_t  terrain_generate_end_time = clock();
//	cout << "Terrain Generation Complete" << endl;
//
//	cudaMemcpyFromSymbol(&terrain_array_host, terrain_array_device, one_side_number * one_side_number * sizeof(char), 0, cudaMemcpyDeviceToHost);
//	clock_t memcpy_end_time = clock();
//	printf("Memcpy Complete\n");
//
//	//add_floor();
//
//	//show_array();
//	cout << "Terrain size : " << one_side_number << " * " << one_side_number << endl;
//	cout << "Terrain Array Size : " << sizeof(terrain_array_host) << " Bytes" << endl;
//	cout << "Terrain Generate Time : " << (double)(terrain_generate_end_time - terrain_generate_start_time) / CLOCKS_PER_SEC << " Seconds" << endl;
//	cout << "Memcpy Time : " << (double)(memcpy_end_time - terrain_generate_end_time) / CLOCKS_PER_SEC << " Seconds" << endl;
//}


//#include <iostream>
//#include <stdio.h>
//#include <random>
//#include <windows.h>
//#include <time.h>
//#include <cuda_runtime.h>
//#include <curand.h>
//#include <curand_kernel.h>
//#include <cooperative_groups.h>
//#include "device_launch_parameters.h"
//using namespace std;
//
//const unsigned int one_side_number = 4000;
//
//const int max_height = 9;
//const int base_floor = 1;
//
//random_device rd;
//default_random_engine dre(rd());
//
//uniform_int_distribution <int>number_of_hills_uid(one_side_number / 10, one_side_number / 5);
//
//char terrain_array_host[one_side_number * one_side_number]{};
//__device__ char terrain_array_device[one_side_number * one_side_number];
//
//void get_device_info()
//{
//	cudaDeviceProp  prop;
//
//	int count;
//	cudaGetDeviceCount(&count);
//
//	for (int i = 0; i < count; i++) {
//		cudaGetDeviceProperties(&prop, i);
//		printf("   --- General Information for device %d ---\n", i);
//		printf("Name:  %s\n", prop.name);
//		printf("Compute capability:  %d.%d\n", prop.major, prop.minor);
//		printf("Clock rate:  %d\n", prop.clockRate);
//		printf("Device copy overlap:  ");
//		if (prop.deviceOverlap)
//			printf("Enabled\n");
//		else
//			printf("Disabled\n");
//		printf("Kernel execution timeout :  ");
//		if (prop.kernelExecTimeoutEnabled)
//			printf("Enabled\n");
//		else
//			printf("Disabled\n");
//		printf("\n");
//
//		printf("   --- Memory Information for device %d ---\n", i);
//		printf("Total global mem:  %ld\n", prop.totalGlobalMem);
//		printf("Total constant Mem:  %ld\n", prop.totalConstMem);
//		printf("Max mem pitch:  %ld\n", prop.memPitch);
//		printf("Texture Alignment:  %ld\n", prop.textureAlignment);
//		printf("\n");
//
//		printf("   --- MP Information for device %d ---\n", i);
//		printf("Multiprocessor count:  %d\n", prop.multiProcessorCount);
//		printf("Shared mem per mp:  %ld\n", prop.sharedMemPerBlock);
//		printf("Registers per mp:  %d\n", prop.regsPerBlock);
//		printf("Threads in warp:  %d\n", prop.warpSize);
//		printf("Max threads per block:  %d\n", prop.maxThreadsPerBlock);
//		printf("Max thread dimensions:  (%d, %d, %d)\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
//		printf("Max grid dimensions:  (%d, %d, %d)\n", prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]);
//		printf("\n");
//	}
//}
//
//void show_array(char* terrain_array_host)
//{
//	for (int i = 0; i < one_side_number; ++i) {
//		for (int j = 0; j < one_side_number; ++j) {
//			printf("%d ", terrain_array_host[i * one_side_number + j]);
//		}
//		printf("\n");
//	}
//}
//
//__device__ 
//int getRand(curandState* s, int A, int B)
//{
//	float rand_int = curand_uniform(s);
//	rand_int = rand_int * (B+1 - A) + A;
//	return rand_int;
//}
//
//__global__
//void add_floor(char* terrain_array_host)
//{
//	for (int i = 0; i < one_side_number * one_side_number; i++) {
//		terrain_array_host[i] += base_floor;
//	}
//}
//
//__global__
//void make_hills_cuda()
//{
//	int id = threadIdx.x + blockIdx.x * blockDim.x;
//	curandState s;
//	curand_init(id, 0, 0, &s);
//
//	/*int hill_location_x = 500;
//	int hill_location_y = 500;
//	int radius = getRand(&s, one_side_number / 20, one_side_number / 10);
//	int height = getRand(&s, 5, max_height);*/
//
//	int hill_location_x = getRand(&s, 0, one_side_number);
//	int hill_location_y = getRand(&s, 0, one_side_number);
//	int radius = getRand(&s, one_side_number / 20, one_side_number / 10);
//	int height = getRand(&s, 5, max_height);
//
//	//printf("START thread: %3d seed: %3d x: %3d y: %3d radius: %3d height: %3d\n", threadIdx.x + blockIdx.x * blockDim.x,id, hill_location_x, hill_location_y, radius, height);
//	printf("%d", terrain_array_device[hill_location_x * one_side_number + hill_location_y]);
//
//	if (terrain_array_device[hill_location_x * one_side_number + hill_location_y]>0) {
//		printf("%d", terrain_array_device[hill_location_x * one_side_number + hill_location_y]);
//		printf("fail\n");
//	}
//	else {
//		for (int j = 0; j < one_side_number; j++) {
//			for (int k = 0; k < one_side_number; k++) {
//				int distance = sqrt(pow(j - hill_location_x, 2) + pow(k - hill_location_y, 2));
//				if (distance <= radius) {
//					terrain_array_device[j * one_side_number + k] += (height - 1) * (radius - distance) / radius + 1;
//					/*if (terrain_array_device[j * one_side_number + k] > max_height) {
//						terrain_array_device[j * one_side_number + k] = max_height;
//					}*/
//				}
//			}
//		}
//	}
//}
//
//
//int main()
//{
//	//get_device_info();
//	/*char* terrain_array_host;
//	cudaMallocHost((void**)&terrain_array_host, one_side_number * one_side_number * sizeof(char));*/
//	//char* terrain_array_host = new char[one_side_number * one_side_number];
//	
//	clock_t terrain_generate_start_time = clock();
//	make_hills_cuda << <60, 5 >> > ();
//
//	cudaMemcpyFromSymbol(&terrain_array_host, terrain_array_device, one_side_number * one_side_number * sizeof(char), 0, cudaMemcpyDeviceToHost);
//	printf("Terrain Generation Complete\n");
//
//	clock_t  terrain_generate_end_time = clock();
//
//	//add_floor << <1, 1 >> > (terrain_array_host);
//
//	//show_array(terrain_array_host);
//	cout << "Terrain size : " << one_side_number << " * " << one_side_number << endl;
//	cout << "Terrain Array Size : " << one_side_number * one_side_number * sizeof(char) << " Bytes" << endl;
//	cout << "Terrain Generate Time : " << (double)(terrain_generate_end_time - terrain_generate_start_time) / CLOCKS_PER_SEC << " Seconds" << endl;
//
//}

//#include <iostream>
//#include <stdio.h>
//#include <random>
//#include <windows.h>
//#include <time.h>
//#include <cuda_runtime.h>
//#include <curand.h>
//#include <curand_kernel.h>
//#include <cooperative_groups.h>
//#include "device_launch_parameters.h"
//using namespace std;
//
//const unsigned int one_side_number = 1000;
//
//const int max_height = 9;
//const int base_floor = 1;
//
//typedef struct hill_info{
//	int x;
//	int y;
//	int radius;
//	int height;
//} HI;
//
//random_device rd;
//default_random_engine dre(rd());
//uniform_int_distribution <int>hills_location(0, one_side_number);
//uniform_int_distribution <int>number_of_hills_uid(one_side_number / 10, one_side_number / 5);
//uniform_int_distribution <int>hill_size_uid(one_side_number / 20, one_side_number / 10);
//uniform_int_distribution <int>height_uid(5, max_height);
//
//char terrain_array_host[one_side_number * one_side_number]{};
//__device__ char terrain_array_device[one_side_number * one_side_number];
//HI hill_location_host[one_side_number * one_side_number];
//__device__ HI hill_location_device[one_side_number * one_side_number];
//
//void get_device_info()
//{
//	cudaDeviceProp  prop;
//
//	int count;
//	cudaGetDeviceCount(&count);
//
//	for (int i = 0; i < count; i++) {
//		cudaGetDeviceProperties(&prop, i);
//		printf("   --- General Information for device %d ---\n", i);
//		printf("Name:  %s\n", prop.name);
//		printf("Compute capability:  %d.%d\n", prop.major, prop.minor);
//		printf("Clock rate:  %d\n", prop.clockRate);
//		printf("Device copy overlap:  ");
//		if (prop.deviceOverlap)
//			printf("Enabled\n");
//		else
//			printf("Disabled\n");
//		printf("Kernel execution timeout :  ");
//		if (prop.kernelExecTimeoutEnabled)
//			printf("Enabled\n");
//		else
//			printf("Disabled\n");
//		printf("\n");
//
//		printf("   --- Memory Information for device %d ---\n", i);
//		printf("Total global mem:  %ld\n", prop.totalGlobalMem);
//		printf("Total constant Mem:  %ld\n", prop.totalConstMem);
//		printf("Max mem pitch:  %ld\n", prop.memPitch);
//		printf("Texture Alignment:  %ld\n", prop.textureAlignment);
//		printf("\n");
//
//		printf("   --- MP Information for device %d ---\n", i);
//		printf("Multiprocessor count:  %d\n", prop.multiProcessorCount);
//		printf("Shared mem per mp:  %ld\n", prop.sharedMemPerBlock);
//		printf("Registers per mp:  %d\n", prop.regsPerBlock);
//		printf("Threads in warp:  %d\n", prop.warpSize);
//		printf("Max threads per block:  %d\n", prop.maxThreadsPerBlock);
//		printf("Max thread dimensions:  (%d, %d, %d)\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
//		printf("Max grid dimensions:  (%d, %d, %d)\n", prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]);
//		printf("\n");
//	}
//}
//
//void show_array(char* terrain_array_host)
//{
//	for (int i = 0; i < one_side_number; ++i) {
//		for (int j = 0; j < one_side_number; ++j) {
//			printf("%d ", terrain_array_host[i * one_side_number + j]);
//		}
//		printf("\n");
//	}
//}
//
//__device__
//int getRand(curandState* s, int A, int B)
//{
//	float rand_int = curand_uniform(s);
//	rand_int = rand_int * (B + 1 - A) + A;
//	return rand_int;
//}
//
//__global__
//void add_floor()
//{
//	for (int i = 0; i < one_side_number * one_side_number; i++) {
//		terrain_array_device[i] += base_floor;
//	}
//}
//
//__global__
//void make_hills_cuda()
//{	
//	int id = threadIdx.x + blockIdx.x * blockDim.x;
//	curandState s;
//
//	int hill_location_x = hill_location_device[id].x;
//	int hill_location_y = hill_location_device[id].y;
//	int radius = hill_location_device[id].radius;
//	int height = hill_location_device[id].height;
//
//	printf("START thread: %3d seed: %3d x: %3d y: %3d radius: %3d height: %3d\n", threadIdx.x + blockIdx.x * blockDim.x,id, hill_location_x, hill_location_y, radius, height);
//	
//	for (int j = 0; j < one_side_number; j++) {
//		for (int k = 0; k < one_side_number; k++) {
//			int distance = sqrt(pow(j - hill_location_x, 2) + pow(k - hill_location_y, 2));
//			if (distance <= radius) {
//				terrain_array_device[j * one_side_number + k] += (height - 1) * (radius - distance) / radius + 1;
//				if (terrain_array_device[j * one_side_number + k] > max_height) {
//					terrain_array_device[j * one_side_number + k] = max_height;
//				}
//			}
//		}
//	}
//}
//
//int make_hill_location()
//{
//	int num_of_hills = number_of_hills_uid(dre);
//
//	cout << "expected num of hill: " << num_of_hills << endl;
//
//	for (int i = 0; i < num_of_hills; i++) {
//		hill_location_host[i].x = hills_location(dre);
//		hill_location_host[i].y = hills_location(dre);
//		hill_location_host[i].radius = hill_size_uid(dre);
//		hill_location_host[i].height = height_uid(dre);
//	}
//	for (int i = 0; i < num_of_hills; i++) {
//		sort(&hill_location_host[0], &hill_location_host[num_of_hills], [](const HI& a, const HI& b) { return a.y < b.y; });
//	}
//	/*for (int i = 0; i < num_of_hills; i++) {
//		cout << hill_location_host[i].x << " " << hill_location_host[i].y << endl;
//	}*/
//	for (int a = 0; a < num_of_hills; a++) {
//		for (int b = 0; b < num_of_hills; b++) {
//			/*cout << pow(hill_location_host[j].x - hill_location_host[k].x, 2) << " " << pow(hill_location_host[j].radius + hill_location_host[k].radius, 2) << endl;
//			cout << pow(hill_location_host[j].y - hill_location_host[k].y, 2) << " " << pow(hill_location_host[j].radius + hill_location_host[k].radius, 2) << endl;
//			cout << endl;*/
//			if (a != b) {
//				if (pow(hill_location_host[a].x - hill_location_host[b].x, 2) - pow(hill_location_host[a].radius + hill_location_host[b].radius, 2) < 0) {
//					if (pow(hill_location_host[a].y - hill_location_host[b].y, 2) - pow(hill_location_host[a].radius + hill_location_host[b].radius, 2) < 0) {
//						//cout << "hit" << endl;
//						for (int i = b; i < num_of_hills; i++) {
//							hill_location_host[i] = hill_location_host[i + 1];
//						}
//						b -= 1;
//						num_of_hills -= 1;
//					}
//				}
//			}
//		}
//	}
//	cudaMemcpyToSymbol(hill_location_device, &hill_location_host, num_of_hills * sizeof(HI), 0, cudaMemcpyHostToDevice);
//	cout << "real num of hill: " << num_of_hills << endl;
//	return num_of_hills;
//}
//
//int main()
//{
//	//get_device_info();
//
//	int num_of_hills = make_hill_location();
//
//	clock_t terrain_generate_start_time = clock();
//	make_hills_cuda << <2, num_of_hills / 2 >> > ();
//
//	cudaMemcpyFromSymbol(&terrain_array_host, terrain_array_device, one_side_number * one_side_number * sizeof(char), 0, cudaMemcpyDeviceToHost);
//	printf("Terrain Generation Complete\n");
//
//	clock_t  terrain_generate_end_time = clock();
//
//	//add_floor << <1, 1 >> > (terrain_array_host);
//
//	//show_array(terrain_array_host);
//	cout << "Terrain size : " << one_side_number << " * " << one_side_number << endl;
//	cout << "Terrain Array Size : " << one_side_number * one_side_number * sizeof(char) << " Bytes" << endl;
//	cout << "Terrain Generate Time : " << (double)(terrain_generate_end_time - terrain_generate_start_time) / CLOCKS_PER_SEC << " Seconds" << endl;
//
//}

//#include <iostream>
//#include <stdio.h>
//#include <random>
//#include <windows.h>
//#include <time.h>
//#include <cuda_runtime.h>
//#include <curand.h>
//#include <curand_kernel.h>
//#include <cooperative_groups.h>
//#include "device_launch_parameters.h"
//using namespace std;
//
//const unsigned int one_side_number = 5000;
//
//const int max_height = 9;
//const int base_floor = 1;
//
//typedef struct hill_info {
//	int x;
//	int y;
//	int radius;
//	int height;
//} HI;
//
//random_device rd;
//default_random_engine dre(rd());
//uniform_int_distribution <int>hills_location(0, one_side_number);
//uniform_int_distribution <int>number_of_hills_uid(one_side_number / 10, one_side_number / 5);
//uniform_int_distribution <int>hill_size_uid(one_side_number / 20, one_side_number / 10);
//uniform_int_distribution <int>height_uid(5, max_height);
//
//char terrain_array_host[one_side_number * one_side_number]{};
//__device__ char terrain_array_device[one_side_number * one_side_number];
//HI hill_location_host[one_side_number * one_side_number];
//__device__ HI hill_location_device[one_side_number * one_side_number];
//
//void get_device_info()
//{
//	cudaDeviceProp  prop;
//
//	int count;
//	cudaGetDeviceCount(&count);
//
//	for (int i = 0; i < count; i++) {
//		cudaGetDeviceProperties(&prop, i);
//		printf("   --- General Information for device %d ---\n", i);
//		printf("Name:  %s\n", prop.name);
//		printf("Compute capability:  %d.%d\n", prop.major, prop.minor);
//		printf("Clock rate:  %d\n", prop.clockRate);
//		printf("Device copy overlap:  ");
//		if (prop.deviceOverlap)
//			printf("Enabled\n");
//		else
//			printf("Disabled\n");
//		printf("Kernel execution timeout :  ");
//		if (prop.kernelExecTimeoutEnabled)
//			printf("Enabled\n");
//		else
//			printf("Disabled\n");
//		printf("\n");
//
//		printf("   --- Memory Information for device %d ---\n", i);
//		printf("Total global mem:  %ld\n", prop.totalGlobalMem);
//		printf("Total constant Mem:  %ld\n", prop.totalConstMem);
//		printf("Max mem pitch:  %ld\n", prop.memPitch);
//		printf("Texture Alignment:  %ld\n", prop.textureAlignment);
//		printf("\n");
//
//		printf("   --- MP Information for device %d ---\n", i);
//		printf("Multiprocessor count:  %d\n", prop.multiProcessorCount);
//		printf("Shared mem per mp:  %ld\n", prop.sharedMemPerBlock);
//		printf("Registers per mp:  %d\n", prop.regsPerBlock);
//		printf("Threads in warp:  %d\n", prop.warpSize);
//		printf("Max threads per block:  %d\n", prop.maxThreadsPerBlock);
//		printf("Max thread dimensions:  (%d, %d, %d)\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
//		printf("Max grid dimensions:  (%d, %d, %d)\n", prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]);
//		printf("\n");
//	}
//}
//
//void show_array(char* terrain_array_host)
//{
//	for (int i = 0; i < one_side_number; ++i) {
//		for (int j = 0; j < one_side_number; ++j) {
//			printf("%d ", terrain_array_host[i * one_side_number + j]);
//		}
//		printf("\n");
//	}
//}
//
//__device__
//int getRand(curandState* s, int A, int B)
//{
//	float rand_int = curand_uniform(s);
//	rand_int = rand_int * (B + 1 - A) + A;
//	return rand_int;
//}
//
//__global__
//void add_floor()
//{
//	for (int i = 0; i < one_side_number * one_side_number; i++) {
//		terrain_array_device[i] += base_floor;
//	}
//}
//
//__global__
//void make_hills_cuda()
//{
//	int id = threadIdx.x + blockIdx.x * blockDim.x;
//	curandState s;
//
//	int hill_location_x = hill_location_device[id].x;
//	int hill_location_y = hill_location_device[id].y;
//	int radius = hill_location_device[id].radius;
//	int height = hill_location_device[id].height;
//	int distance{};
//
//	printf("START thread: %3d seed: %3d x: %3d y: %3d radius: %3d height: %3d\n", threadIdx.x + blockIdx.x * blockDim.x, id, hill_location_x, hill_location_y, radius, height);
//
//	for (int x = hill_location_x - radius; x <= hill_location_x + radius; x++) {
//		for (int y = hill_location_y - radius ; y <= hill_location_y + radius; y++) {
//			distance = sqrt(pow(x - hill_location_x, 2) + pow(y - hill_location_y, 2));
//			if (distance <= radius) {
//				//printf("%d %d\n", x, y);
//				terrain_array_device[x * one_side_number + y] += (height - 1) * (radius - distance) / radius + 1;
//				//printf("%d\n",(height - 1) * (radius - distance) / radius + 1);
//				if (terrain_array_device[x * one_side_number + y] > max_height) {
//					terrain_array_device[x * one_side_number + y] = max_height;
//				}
//			}
//		}
//	}
//}
//
//int make_hill_location()
//{
//	int num_of_hills = number_of_hills_uid(dre);
//
//	cout << "expected num of hill: " << num_of_hills << endl;
//
//	for (int i = 0; i < num_of_hills; i++) {
//		hill_location_host[i].x = hills_location(dre);
//		hill_location_host[i].y = hills_location(dre);
//		hill_location_host[i].radius = hill_size_uid(dre);
//		hill_location_host[i].height = height_uid(dre);
//	}
//	for (int i = 0; i < num_of_hills; i++) {
//		sort(&hill_location_host[0], &hill_location_host[num_of_hills], [](const HI& a, const HI& b) { return a.y < b.y; });
//	}
//
//	for (int a = 0; a < num_of_hills; a++) {
//		for (int b = 0; b < num_of_hills; b++) {
//			if (a != b) {
//				if (pow(hill_location_host[a].x - hill_location_host[b].x, 2) - pow(hill_location_host[a].radius + hill_location_host[b].radius, 2) < 0) {
//					if (pow(hill_location_host[a].y - hill_location_host[b].y, 2) - pow(hill_location_host[a].radius + hill_location_host[b].radius, 2) < 0) {
//						//cout << "hit" << endl;
//						for (int i = b; i < num_of_hills; i++) {
//							hill_location_host[i] = hill_location_host[i + 1];
//						}
//						b -= 1;
//						num_of_hills -= 1;
//					}
//				}
//			}
//		}
//	}
//	for (int i = 0; i < num_of_hills; i++) {
//		if (hill_location_host[i].x - hill_location_host[i].radius < 0) {
//			for (int j = i; j < num_of_hills; j++) {
//				hill_location_host[j] = hill_location_host[j + 1];
//			}
//			i -= 1;
//			num_of_hills -= 1;
//		}
//		else if (hill_location_host[i].x + hill_location_host[i].radius >= one_side_number) {
//			for (int j = i; j < num_of_hills; j++) {
//				hill_location_host[j] = hill_location_host[j + 1];
//			}
//			i -= 1;
//			num_of_hills -= 1;
//		}
//		else if (hill_location_host[i].y - hill_location_host[i].radius < 0) {
//			for (int j = i; j < num_of_hills; j++) {
//				hill_location_host[j] = hill_location_host[j + 1];
//			}
//			i -= 1;
//			num_of_hills -= 1;
//		}
//		else if (hill_location_host[i].y + hill_location_host[i].radius >= one_side_number) {
//			for (int j = i; j < num_of_hills; j++) {
//				hill_location_host[j] = hill_location_host[j + 1];
//			}
//			i -= 1;
//			num_of_hills -= 1;
//		}
//	}
//	cudaMemcpyToSymbol(hill_location_device, &hill_location_host, num_of_hills * sizeof(HI), 0, cudaMemcpyHostToDevice);
//	cout << "real num of hill: " << num_of_hills << endl;
//	return num_of_hills;
//}
//
//int main()
//{
//	//get_device_info();
//
//	int num_of_hills = make_hill_location();
//
//	clock_t terrain_generate_start_time = clock();
//	make_hills_cuda << <num_of_hills/2, 2>> > ();
//	//add_floor << <1, 1 >> > ();
//
//	cudaMemcpyFromSymbol(&terrain_array_host, terrain_array_device, one_side_number * one_side_number * sizeof(char), 0, cudaMemcpyDeviceToHost);
//	printf("Terrain Generation Complete\n");
//
//	clock_t  terrain_generate_end_time = clock();
//
//	//show_array(terrain_array_host);
//	
//	cout << "Terrain size : " << one_side_number << " * " << one_side_number << endl;
//	cout << "Terrain Array Size : " << one_side_number * one_side_number * sizeof(char) << " Bytes" << endl;
//	cout << "Terrain Generate Time : " << (double)(terrain_generate_end_time - terrain_generate_start_time) / CLOCKS_PER_SEC << " Seconds" << endl;
//}


//2차원 배열
#include <iostream>
#include <stdio.h>
#include <random>
#include <windows.h>
#include <time.h>
#include <cuda_runtime.h>
#include <curand.h>
#include <curand_kernel.h>
#include <cooperative_groups.h>
#include "device_launch_parameters.h"
using namespace std;

const unsigned int one_side_number = 1000;

const int max_height = 9;
const int base_floor = 1;

typedef struct hill_info {
	int x;
	int y;
	int radius;
	int height;
} HI;

random_device rd;
default_random_engine dre(rd());
uniform_int_distribution <int>hills_location(0, one_side_number);
uniform_int_distribution <int>number_of_hills_uid(one_side_number / 10, one_side_number / 5);
uniform_int_distribution <int>hill_size_uid(one_side_number / 20, one_side_number / 10);
uniform_int_distribution <int>height_uid(5, max_height);

char terrain_array_host[one_side_number][one_side_number]{};
__device__ char terrain_array_device[one_side_number][one_side_number];

HI hill_location_host[one_side_number * one_side_number];
__device__ HI hill_location_device[one_side_number * one_side_number];

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

void show_array()
{
	for (int x = 0; x < one_side_number; x++) {
		for (int y = 0; y < one_side_number; y++) {
			printf("%d ", terrain_array_host[x][y]);
		}
		printf("\n");
	}
}

__global__
void add_floor()
{
	for (int x = 0; x < one_side_number; x++) {
		for (int y = 0; y < one_side_number; y++) {
			terrain_array_device[x][y] += base_floor;
		}
	}
}

__global__
void make_hills_cuda()
{
	int id = threadIdx.x + blockIdx.x * blockDim.x;
	curandState s;

	int hill_location_x = hill_location_device[id].x;
	int hill_location_y = hill_location_device[id].y;
	int radius = hill_location_device[id].radius;
	int height = hill_location_device[id].height;
	int distance{};

	printf("START thread: %3d seed: %3d x: %3d y: %3d radius: %3d height: %3d\n", threadIdx.x + blockIdx.x * blockDim.x, id, hill_location_x, hill_location_y, radius, height);

	for (int x = hill_location_x - radius; x <= hill_location_x + radius; x++) {
		for (int y = hill_location_y - radius; y <= hill_location_y + radius; y++) {
			distance = sqrt(pow(x - hill_location_x, 2) + pow(y - hill_location_y, 2));
			if (distance <= radius) {
				//printf("%d %d\n", x, y);
				terrain_array_device[x][y] += (height - 1) * (radius - distance) / radius + 1;
				//printf("%d\n",(height - 1) * (radius - distance) / radius + 1);
				if (terrain_array_device[x][y] > max_height) {
					terrain_array_device[x][y] = max_height;
				}
			}
		}
	}
}

int make_hill_location()
{
	int num_of_hills = number_of_hills_uid(dre);

	cout << "expected num of hill: " << num_of_hills << endl;

	for (int i = 0; i < num_of_hills; i++) {
		hill_location_host[i].x = hills_location(dre);
		hill_location_host[i].y = hills_location(dre);
		hill_location_host[i].radius = hill_size_uid(dre);
		hill_location_host[i].height = height_uid(dre);
	}
	for (int i = 0; i < num_of_hills; i++) {
		sort(&hill_location_host[0], &hill_location_host[num_of_hills], [](const HI& a, const HI& b) { return a.y < b.y; });
	}
	for (int a = 0; a < num_of_hills; a++) {
		for (int b = 0; b < num_of_hills; b++) {
			if (a != b) {
				if (pow(hill_location_host[a].x - hill_location_host[b].x, 2) - pow(hill_location_host[a].radius + hill_location_host[b].radius, 2) < 0) {
					if (pow(hill_location_host[a].y - hill_location_host[b].y, 2) - pow(hill_location_host[a].radius + hill_location_host[b].radius, 2) < 0) {
						//cout << "hit" << endl;
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
	cudaMemcpyToSymbol(hill_location_device, &hill_location_host, num_of_hills * sizeof(HI), 0, cudaMemcpyHostToDevice);
	cout << "real num of hill: " << num_of_hills << endl;
	return num_of_hills;
}

int main()
{
	//get_device_info();

	int num_of_hills = make_hill_location();

	clock_t terrain_generate_start_time = clock();
	make_hills_cuda << <num_of_hills / 2, 2 >> > ();
	//add_floor << <1, 1 >> > ();

	cudaMemcpyFromSymbol(&terrain_array_host, terrain_array_device, one_side_number * one_side_number * sizeof(char), 0, cudaMemcpyDeviceToHost);
	printf("Terrain Generation Complete\n");

	clock_t  terrain_generate_end_time = clock();

	show_array();

	cout << "Terrain size : " << one_side_number << " * " << one_side_number << endl;
	cout << "Terrain Array Size : " << one_side_number * one_side_number * sizeof(char) << " Bytes" << endl;
	cout << "Terrain Generate Time : " << (double)(terrain_generate_end_time - terrain_generate_start_time) / CLOCKS_PER_SEC << " Seconds" << endl;
}
