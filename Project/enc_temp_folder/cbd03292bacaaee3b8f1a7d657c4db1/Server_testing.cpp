// Fill out your copyright notice in the Description page of Project Settings.
#include "Server_testing.h"
#include <math.h>
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include <future>
#include <thread>
#include <mutex>
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Components/InstancedStaticMeshComponent.h"

//AMyPlayerController* my_controller;

AServer_testing::AServer_testing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AServer_testing::BeginPlay()
{
	Super::BeginPlay();

	//FSocketThread* SocketThread = new FSocketThread(FString("127.0.0.1"), 9000);
	//bool connected = SocketThread->Init();
	//UE_LOG(LogTemp, Warning, TEXT("Connected: %d"), connected);
	
	//Citizen
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	Citizens = GetWorld()->SpawnActor<ACitizen>(Location, Rotation, SpawnInfo);
	Citizens->Initialize(Citizen_Actor,EnemyCitizenActor);

	//Set Size of Terrain Array 
	Terrain2DArray.SetNum(MapSizeX);
	for (int i = 0; i < MapSizeX; ++i){
		Terrain2DArray[i].SetNum(MapSizeY);
	}

	//Init Mesh Terrain
	TerrainActor = GetWorld()->SpawnActor<AMeshTerrain>(Location, Rotation, SpawnInfo);
	TerrainActor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TerrainActor->InitializeMeshTerrain(TerrainMaterial);

	
	/*wcout.imbue(locale("korean"));*/
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
		recv(s_socket, (char*)&Citizens->temp_Actor, sizeof(FActor_location_rotation), 0);
		FActor_location_rotation tmp;
		players_list.Add(i, tmp);
		TF_set(players_list[i].location, Citizens->temp_Actor.location);
		Fcitizen_struct temp;
		Citizens->My_Citizen.Add(i, temp);
		for (int j = 0; j < 10; ++j)
		{
			recv(s_socket, (char*)&Citizens->temp_Actor, sizeof(FCitizen_sole), 0);

			Citizens->citizen_set(i, j);
		}
	}

	Citizens->Spawn_Citizen();
	for (int i = 0; i < MAXPLAYER * 10; ++i){
		Fresources_actor temp_resource;
		recv(s_socket, (char*)&temp_resource, sizeof(Fresources_actor), 0);
		resources_create_landscape.Add(i, temp_resource);
		resoure_set(resources_create_landscape[i], temp_resource);
	}
	first_recv_send = true;

	UE_LOG(LogTemp, Log, TEXT("connected to server"));
	start_t = high_resolution_clock::now();
}

// Called every frame
void AServer_testing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	
	ret = send(s_socket, (char*)&Citizens->Citizen_moving, (int)sizeof(FCitizen_moving), 0);
	

	ret = recv(s_socket, (char*)&sunangle, (int)sizeof(Fthree_float), 0);
	

	for (int i = 0; i < MAXPLAYER; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			recv(s_socket, (char*)&Citizens->temp_Actor, sizeof(FCitizen_sole), 0);

			Citizens->TF_set(Citizens->My_Citizen[i].citizen_location_rotation[j].location, Citizens->temp_Actor.location);

		}
	}

	Citizens->Citizen_Moving();

	/*for (int i = 0; i < MAXPLAYER * 10; ++i)
	{
		Fresources_actor temp_resource;
		recv(s_socket, (char*)&temp_resource, sizeof(Fresources_actor), 0);
		resoure_set(resources_create_landscape[i], temp_resource);
	}*/

	//카메라 위치 및 입력보내버리기
	send(s_socket, (char*)&my_key_input, sizeof(Fkeyboard_input), 0);
	recv(s_socket, (char*)&my_camera_location, sizeof(Fthree_float), 0);


	//자원 받기
	recv(s_socket, (char*)&resources, sizeof(int) * 5, 0);
	oil_count = resources[0], water_count = resources[1], iron_count = resources[2], food_count = resources[3], wood_count = resources[4];

	//Recv Terrain
	//ret = recv(s_socket, (char*)&Terrain2DArray, (int)sizeof(char) * MapSizeX * MapSizeY, 0);
	for (int i = 0; i < MapSizeX; i++) {
		ret = recv(s_socket, (char*)&terrain_recv_array, (int)sizeof(char) * MapSizeY, 0);
		if (SOCKET_ERROR == ret){
			return;
		}
		for (int j = 0; j < MapSizeY; j++)
		{
			Terrain2DArray[i][j] = terrain_recv_array[j];
		}
	}

	
	TerrainActor->UpdateMeshTerrain(Terrain2DArray);
	
}



void AServer_testing::TF_set(Fthree_float& a, Fthree_float& b)
{
	a.x = b.x;
	a.y = b.y;
	a.z = b.z;
}


void AServer_testing::resoure_set(Fresources_actor& a, Fresources_actor& b)
{
	a.count = b.count;
	a.type = b.type;
	TF_set(a.location, b.location);
}
