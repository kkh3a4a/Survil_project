// Fill out your copyright notice in the Description page of Project Settings.

#include <math.h>
#include "MyPlayerController.h"
#include "Server_testing.h"
#include "Kismet/GameplayStatics.h"
#include "citizen.h"
// Sets default values

//AMyPlayerController* my_controller;

AServer_testing::AServer_testing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	for (int i = 0; i < map_size * map_size; ++i)
	{
		int_array.Emplace(1);
	}
}

int8 AServer_testing::get_height(int32 x, int32 y)
{
	return terrain_array[x][y];
}

// Called when the game starts or when spawned
void AServer_testing::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> CitizensToFind;
	wcout.imbue(locale("korean"));
	ret = WSAStartup(MAKEWORD(2, 2), &WSAData);
	s_socket = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	ret = inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

	//connect();
	ret = connect(s_socket, reinterpret_cast<sockaddr*> (&server_addr), sizeof(server_addr));
	int cnt = 0;


	for (int i = 0; i < MAXPLAYER; ++i) {
		Citizen_num++;
		recv(s_socket, (char*)&temp_Actor, sizeof(FActor_location_rotation), 0);
		FActor_location_rotation tmp;
		players_list.Add(i, tmp);

		players_list[i].location.x = temp_Actor.location.x;
		players_list[i].location.y = temp_Actor.location.y;
		players_list[i].location.z = temp_Actor.location.z;
		//UE_LOG(LogTemp, Log, TEXT("player : %f %f"), players_list[i].location.x, players_list[i].location.y);
		Fcitizen_struct temp;
		citizen.Add(i, temp);
		for (int j = 0; j < 10; ++j)
		{
			recv(s_socket, (char*)&temp_Actor, sizeof(FActor_location_rotation), 0);

			FActor_location_rotation citizentemp;
			citizentemp.location.x = temp_Actor.location.x;
			citizentemp.location.y = temp_Actor.location.y;
			citizentemp.location.z = temp_Actor.location.z;
			citizen[i].citizen_location_rotation.Add(citizentemp);
			citizen[i].citizen_location_rotation[j].location.x = temp_Actor.location.x;
			citizen[i].citizen_location_rotation[j].location.y = temp_Actor.location.y;
			citizen[i].citizen_location_rotation[j].location.z = temp_Actor.location.z;
			//UE_LOG(LogTemp, Log, TEXT("citizen %d %d : %f %f"), i , j, citizen[i].citizen_location_rotation[j].location.x, citizen[i].citizen_location_rotation[j].location.y);
		}
	}
	for (auto& a : citizen)
	{
		for (int j = 0; j < 10; ++j)
		{
			//UE_LOG(LogTemp, Log, TEXT("%f %f"), a.Value.citizen_location_rotation[j].location.x, a.Value.citizen_location_rotation[j].location.y);
		}
	}

	first_recv_send = true;

	UE_LOG(LogTemp, Log, TEXT("connected to server"));
	start_t = high_resolution_clock::now();
}

// Called every frame
void AServer_testing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	steady_clock::time_point end_t = high_resolution_clock::now();
	//UE_LOG(LogTemp, Log, TEXT("%s : %lf, %lf"), *MouseInput.name, MouseInput.location.x, MouseInput.location.y);
	//UE_LOG(LogTemp, Log, TEXT("%d"), test);
	//my_controller->testFunction();
	//UE_LOG(LogTemp, Log, TEXT("%d"), test);
	if (duration_cast<milliseconds>(end_t - start_t).count() > 50) {

		start_t = high_resolution_clock::now();
		ret = send(s_socket, (char*)&Citizen_moving, (int)sizeof(FCitizen_moving), 0);
		if (SOCKET_ERROR == ret)
		{
			return;
		}

		ret = recv(s_socket, (char*)&sunangle, (int)sizeof(Fthree_float), 0);
		if (SOCKET_ERROR == ret)
		{
			return;
		}
		for (int i = 0; i < MAXPLAYER; ++i) {
			for (int j = 0; j < 10; ++j)
			{
				recv(s_socket, (char*)&temp_Actor, sizeof(FActor_location_rotation), 0);

				citizen[i].citizen_location_rotation[j].location.x = temp_Actor.location.x;
				citizen[i].citizen_location_rotation[j].location.y = temp_Actor.location.y;
				citizen[i].citizen_location_rotation[j].location.z = temp_Actor.location.z;

				/*FVector citizen_tmp = { temp_Actor.location.x,temp_Actor.location.y,temp_Actor.location.z };
				if (My_Citizen[i].citizen_AActor[j] != nullptr)
				{
					My_Citizen[i].citizen_AActor[j]->SetActorLocation(citizen_tmp);
				}*/
				//UE_LOG(LogTemp, Log, TEXT("citizen %d : %f %f"), i, citizen[i].citizen_location_rotation[j].location.x, citizen[i].citizen_location_rotation[j].location.y);
			}
		}
		//UE_LOG(LogTemp, Log, TEXT("%d %lf %lf"), cnt, MYplayer_controller->MouseInput.location.x, MYplayer_controller->MouseInput.location.y)
		//UE_LOG(LogTemp, Log, TEXT("%d %lf %lf"), cnt, MouseInput.location.x, MouseInput.location.y)
		
		//===================
		FOneArray temp_array;
		terrain_array.Init(temp_array, map_size);
		

		for (int i = 0; i < map_size; i++)
		{
			//terrain_array[i].Empty();
			ret = recv(s_socket, (char*)&terrain_2d_array, (int)(sizeof(char) * map_size), 0);
			if (SOCKET_ERROR == ret)
			{
				return;
			}
			for (int j = 0; j < map_size; j++)
			{
				terrain_array[i].Add(terrain_2d_array[j]);
				//UE_LOG(LogTemp, Log, TEXT("%d %d %d"), i, j, terrain_array[i][j]);
				if(int_array[i*map_size+j] != terrain_array[i][j])
					int_array[i*map_size+j] = (terrain_array[i][j]);
				//UE_LOG(LogTemp, Log, TEXT("int array[%d] [%d] : %d"), i, j, int_array[i * 64 + j]);
			}
		}
		UE_LOG(LogTemp, Log, TEXT("terrain_array : %d"),sizeof(terrain_array));
		sands_size = sqrt(int_array.Num());
		//UE_LOG(LogTemp, Log, TEXT("int array size : %d\n"), int_array.Num());

		//UE_LOG(LogTemp, Log, TEXT("map load once"));
		//=====================
	}


	//UE_LOG(LogTemp, Log, TEXT("%f, %f, %f"), test_Actor.location.x , test_Actor.location.y, test_Actor.location.z);
	//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow, TEXT(" %d, %d, %d", sunangle.x, sunangle.y, sunangle.z));

}
//
//DWORD WINAPI AServer_testing::Angle_Receiver(LPVOID arg)
//{
//	while (1) {
//		if (SOCKET_ERROR == ret)
//		{
//			break;
//		}
//		ret = send(s_socket, (char*)&sunangle, (int)sizeof(SunAngle), 0);
//	}
//}

