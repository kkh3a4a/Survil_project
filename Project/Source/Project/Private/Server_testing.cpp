// Fill out your copyright notice in the Description page of Project Settings.
#include "Server_testing.h"
#include <math.h>
#include "MyPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "citizen.h"
// Sets default values

//AMyPlayerController* my_controller;

AServer_testing::AServer_testing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

int32 AServer_testing::get_height(int32 x, int32 y)
{
	if (x < 0 || x >= map_size || y < 0 || y >= map_size){
		return 1;
	}
	else if (terrain_2d_array.Num() == 0){
		return 1;
	}
	else{
		return terrain_2d_array[x][y];
	}
}

void AServer_testing::SpawnISM()
{
	InstancedTerrainBlock = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	

	FRotator SpawnRotation = FRotator(0, 0, 0);
	FVector SpawnLocation = FVector(0, 0, 0);
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 200; j++) {
			SpawnLocation = FVector(i * 100.1, j * 100.1, 1000);
			GetWorld()->SpawnActor<AActor>(TerrainBlock, SpawnLocation, SpawnRotation);
			//ISM->AddInstance(FTransform(SpawnRotation, SpawnLocation, FVector(1, 1, 1)));
			UE_LOG(LogTemp, Warning, TEXT("SpawnedActor"));
		}
	}
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

	SpawnISM();


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
			recv(s_socket, (char*)&temp_Actor, sizeof(FCitizen_sole), 0);

			FCitizen_sole citizentemp;
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
			UE_LOG(LogTemp, Log, TEXT("%f %f"), a.Value.citizen_location_rotation[j].location.x, a.Value.citizen_location_rotation[j].location.y);
		}
	}
	for (int i = 0; i < MAXPLAYER * 10; ++i)
	{
		Fresources_actor temp_resource;
		recv(s_socket, (char*)&temp_resource, sizeof(Fresources_actor), 0);
		resources_create_landscape.Add(i, temp_resource);
		resources_create_landscape[i].count = temp_resource.count;
		resources_create_landscape[i].type = temp_resource.type;
		resources_create_landscape[i].location.x = temp_resource.location.x;
		resources_create_landscape[i].location.y = temp_resource.location.y;
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
		for (int i = 0; i < MAXPLAYER; ++i) 
		{
			for (int j = 0; j < 10; ++j)
			{
				recv(s_socket, (char*)&temp_Actor, sizeof(FCitizen_sole), 0);

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


		for (int i = 0; i < MAXPLAYER * 10; ++i)
		{
			Fresources_actor temp_resource;
			recv(s_socket, (char*)&temp_resource, sizeof(Fresources_actor), 0);
			resources_create_landscape[i].count = temp_resource.count;
			resources_create_landscape[i].type = temp_resource.type;
			resources_create_landscape[i].location.x = temp_resource.location.x;
			resources_create_landscape[i].location.y = temp_resource.location.y;
		}

		//카메라 위치 및 입력보내버리기
		send(s_socket, (char*)&my_key_input, sizeof(Fkeyboard_input), 0);
		recv(s_socket, (char*)&my_camera_location, sizeof(Fthree_float), 0);


		//자원 받기
		recv(s_socket, (char*)&resources, sizeof(int)*5, 0);
		
		
		oil_count = resources[0], water_count = resources[1], iron_count = resources[2], food_count = resources[3], wood_count = resources[4];
		//===================
		FOneArray temp_array;
		terrain_2d_array.Init(temp_array, map_size);
		

		for (int i = 0; i < map_size; i++)
		{
			//terrain_array[i].Empty();
			ret = recv(s_socket, (char*)&terrain_recv_array, (int)(sizeof(char) * map_size), 0);
			if (SOCKET_ERROR == ret)
			{
				return;
			}
			for (int j = 0; j < map_size; j++)
			{
				terrain_2d_array[i].Add(terrain_recv_array[j]);
			}
		}
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

