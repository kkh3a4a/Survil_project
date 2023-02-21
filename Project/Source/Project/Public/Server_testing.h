// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Windows/AllowWindowsPlatformTypes.h"
// include header with included Windows.h
#include<WS2tcpip.h>
#include<iostream>
#include <chrono>
#include "global.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "MeshTerrain.h"
#include "NetworkingThread.h"
#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "GameFramework/Actor.h"
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
	int resources[5] = {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int oil_count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int water_count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int iron_count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int food_count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int wood_count = 0;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, FActor_location_rotation> players_list;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, Fcitizen_struct> citizen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCitizen_sole temp_Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, FAActor_struct> My_Citizen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, Fresources_actor> resources_create_landscape;

	int My_Citizen_Num = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Fthree_float my_camera_location;

	steady_clock::time_point start_t;
	WSADATA WSAData;
	SOCKET s_socket;
	HANDLE hThread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int max_player_cnt = MAXPLAYER;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* My_Town;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> citizen_Blueprint;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//FActor_location_rotation MouseInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCitizen_moving Citizen_moving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	Fkeyboard_input my_key_input;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	Fthree_float sunangle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActor_location_rotation test_Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Citizen_num = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool first_recv_send = false;

	int8 terrain_recv_array[map_size];
	TArray<TArray<int8>> Terrain2DArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* TerrainMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> TerrainBlock;
	
	AMeshTerrain* TerrainActor;
};

