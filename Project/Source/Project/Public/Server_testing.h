// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Windows/AllowWindowsPlatformTypes.h"
// include header with included Windows.h
#include<WS2tcpip.h>
#include<iostream>
#include <chrono>
#include "global.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "NetworkingThread.h"
#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "GameFramework/Actor.h"
#include "Citizen.h"
#include "MyTown.h"
#include "MeshTerrain.h"
#include "Temperature.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Server_testing.generated.h"

using namespace std;
using namespace chrono;

UCLASS()
class PROJECT_API AServer_testing : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AServer_testing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	const char* SERVER_ADDR = "127.0.0.1";
	const short SERVER_PORT = 9000;
	int ret = 0;

	bool Is_send_UI_input = false;
	bool IsConnect = false;
	bool IsFirstSend = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUI_Input UI_Input;

	//////////////UI때문에 여기있어야함
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
		int oil_count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
		int water_count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
		int iron_count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
		int food_count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
		int wood_count = 0;

	volatile int maxplayer_cnt = 0;
	volatile int trash_value = 0;


	TMap<int, FActor_location_rotation*> players_list;

	

	steady_clock::time_point start_t;
	WSADATA WSAData;
	SOCKET s_socket;
	HANDLE hThread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int max_player_cnt = MAXPLAYER;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* My_Town;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Fkeyboard_input my_key_input;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Citizen_num = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool first_recv_send = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CitizenNoJobCnt = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Citizen")
		TSubclassOf<AActor>Citizen_Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Citizen")
		TSubclassOf<AActor>EnemyCitizenActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TSubclassOf<AActor>WellPump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TSubclassOf<AActor>OilActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TSubclassOf<AActor>WaterActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TSubclassOf<AActor>IronActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TSubclassOf<AActor>FoodActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TSubclassOf<AActor>WoodActor;
	//citizen
	ACitizen* Citizens;
	AMyTown* MyTown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Fthree_float sunangle;


	int8 Terrain2DArray[MapSizeX][MapSizeY];
	int8 TerrainTemperature[MapSizeX][MapSizeY];
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
		UMaterialInstance* TerrainMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature")
		UMaterial* TemperatureMaterial;

	ATemperature* decal;
	
	AMeshTerrain* TerrainActor;
	FSocketThread* Networking;
	FRunnableThread* NetworkingThread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Fthree_float CurrentLocation;
	
	ACameraActor* CameraActor;

	FFirstSendServer FirstSendServer;

	void TF_set(Fthree_float& a, Fthree_float& b);

	void resoure_set(Fresources_actor& a, Fresources_actor& b);

	int connecting();

	bool FirstSend();
};




