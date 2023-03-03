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
	
	/*FSocketThread* SocketThread = new FSocketThread(FString("127.0.0.1"), 9000);
	bool connected = SocketThread->Init();
	UE_LOG(LogTemp, Warning, TEXT("Connected: %d"), connected);*/

	//Citizen
	FActorSpawnParameters SpawnInfo;
	Citizens = GetWorld()->SpawnActor<ACitizen>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	Citizens->Initialize(Citizen_Actor, EnemyCitizenActor);

	//resource
	MyTown = GetWorld()->SpawnActor<AMyTown>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	MyTown->Initialize(WellPump, OilActor, WaterActor, IronActor, FoodActor, WoodActor);

	//Init Mesh Terrain
	TerrainActor = GetWorld()->SpawnActor<AMeshTerrain>(FVector(0, 0, 0), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	TerrainActor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TerrainActor->InitializeMeshTerrain(TerrainMaterialInstance);

	//Spawn Decal
	decal = GetWorld()->SpawnActor<ATemperature>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	decal->Initiaize(TemperatureMaterial);
	decal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	UI_Input.resouce_input.ResourceNum = -1;
	UI_Input.resouce_input.CitizenCountAdd = false;
	UI_Input.resouce_input.CitizenCountSub = false;
	
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
	
}

// Called every frame
void AServer_testing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!IsFirstSend)
	{
		if (FirstSend())
			IsFirstSend = true;
	}
	else {
		recv(s_socket, (char*)&FirstSendServer, sizeof(FirstSendServer), 0);
		//ret = recv(s_socket, (char*)&sunangle, (int)sizeof(Fthree_float), 0);
		//for (int i = 0; i < MAXPLAYER; ++i)
		//{
		//	for (int j = 0; j < 10; ++j)
		//	{
		//		recv(s_socket, (char*)&Citizens->My_Citizen[i][j], sizeof(FCitizen_sole), 0);
		//	}
		//}
		//for (int i = 0; i < MAXPLAYER * 10; ++i)
		//{
		//	Fresources_actor temp_resource;
		//	recv(s_socket, (char*)&temp_resource, sizeof(Fresources_actor), 0);
		//	MyTown->UpdateResource(MyTown->resources_create_landscape[i], temp_resource, i);
		//}
		//recv(s_socket, (char*)&CurrentLocation, sizeof(Fthree_float), 0);
		////자원 받기
		//recv(s_socket, (char*)&resources, sizeof(int) * 5, 0);
		//oil_count = resources[0], water_count = resources[1], iron_count = resources[2], food_count = resources[3], wood_count = resources[4];
		////Recv Terrain
		//for (int i = 0; i < MapSizeX; i++) {
		//	ret = recv(s_socket, (char*)&Terrain2DArray[i], sizeof(char) * MapSizeY, 0);
		//	if (SOCKET_ERROR == ret) {
		//		return;
		//	}
		//}
		////Recv Temperature
		//for (int i = 0; i < MapSizeX; i++) {
		//	ret = recv(s_socket, (char*)&TerrainTemperature[i], sizeof(char) * MapSizeY, 0);
		//	if (SOCKET_ERROR == ret) {
		//		return;
		//	}
		//}

		////카메라 위치 및 입력보내버리기
		//ret = send(s_socket, (char*)&Citizens->Citizen_moving, (int)sizeof(FCitizen_moving), 0);
		//send(s_socket, (char*)&my_key_input, sizeof(Fkeyboard_input), 0);

		////보냈는가 안보냈는가 확인
		//if (Is_send_UI_input == false)
		//{
		//	if (UI_Input.resouce_input.CitizenCountAdd || UI_Input.resouce_input.CitizenCountSub)
		//	{
		//		Is_send_UI_input = true;
		//	}
		//}
		//else if (Is_send_UI_input == true)
		//{
		//	if (!UI_Input.resouce_input.CitizenCountAdd && !UI_Input.resouce_input.CitizenCountSub)
		//	{
		//		Is_send_UI_input = false;
		//	}
		//	UI_Input.resouce_input.CitizenCountAdd = false;
		//	UI_Input.resouce_input.CitizenCountSub = false;
		//}
		//send(s_socket, (char*)&UI_Input, sizeof(FUI_Input), 0);

		oil_count = FirstSendServer.MyResource[0], water_count = FirstSendServer.MyResource[1], iron_count = FirstSendServer.MyResource[2], food_count = FirstSendServer.MyResource[3], wood_count = FirstSendServer.MyResource[4];
		TF_set(CurrentLocation, players_list[0]->location);
		TF_set(sunangle, FirstSendServer.SunAngle);
		Citizens->Citizen_Moving();
		SetActorLocation(FVector(CurrentLocation.x - MapSizeX * 100 / 2, CurrentLocation.y - MapSizeY * 100 / 2, CurrentLocation.z));
	}
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
}

int AServer_testing::connecting()
{

	
	return 1;
}

bool AServer_testing::FirstSend()
{
	if (!(maxplayer_cnt == MAXPLAYER))
	{
		recv(s_socket, (char*)&maxplayer_cnt, sizeof(int), 0);
		send(s_socket, (char*)&trash_value, sizeof(int), 0);
		return 0;
	}

	

	recv(s_socket, (char*)&FirstSendServer, sizeof(FirstSendServer), 0);

	for (int i = 0; i < MAXPLAYER; ++i) {
		players_list.Add(i, &(FirstSendServer.player_info));
	}

	Citizens->citizen_set(FirstSendServer);

	Citizens->Spawn_Citizen();

	MyTown->SpawnTown(players_list);

	MyTown->SpawnResource(FirstSendServer);
	first_recv_send = true;

	UE_LOG(LogTemp, Log, TEXT("connected to server"));
	start_t = high_resolution_clock::now();

	Citizens->Citizen_Moving();
	return 1;
}
