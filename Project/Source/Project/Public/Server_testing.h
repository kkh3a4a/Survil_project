// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Windows/AllowWindowsPlatformTypes.h"

// include header with included Windows.h
#include<WS2tcpip.h>
#include<iostream>
#include <chrono>
#include "global.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Server_testing.generated.h"

using namespace std;
using namespace chrono;
const int map_size = 50;

USTRUCT(Atomic, BlueprintType)
struct FOneArray
{
	GENERATED_BODY()
public:
	TArray<int8> one_side_array;

	int8 operator[] (int32 i)
	{
		return one_side_array[i];
	}

	void Init(int32 size)
	{
		one_side_array.Init(0, size);
	}

	void SetNum(int32 size)
	{
		one_side_array.SetNum(size);
	}
	
	void Copy (const TArray<int8>* other)
	{
		memcpy(&one_side_array, other, map_size * sizeof(int8));
	}

	void Set(int i, int8 value)
	{
		one_side_array[i] = value;
	}

	void Add(int8 item)
	{
		one_side_array.Add(item);
	}

	int32 Num()
	{
		return one_side_array.Num();
	}

	int32 GetTypeSize()
	{
		return one_side_array.GetTypeSize();
	}
	int32 GetAllocatedSize()
	{
		return one_side_array.GetAllocatedSize();
	}
	void Empty()
	{
		one_side_array.Empty();
	}
};

UCLASS()
class PROJECT_API AServer_testing : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AServer_testing();
	
	UFUNCTION(BlueprintCallable)
	int32 get_height(int32 x, int32 y);

	UFUNCTION()
	void SpawnTerrain();

	UFUNCTION()
	void UpdateTerrainHeight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	const char* SERVER_ADDR = "127.0.0.1";
	const short SERVER_PORT = 9000;
	const int BUFSIZE = 256;
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
	
	

	int32 UpdateTerrainIter{};
	int8 terrain_recv_array[map_size];
	//FOneArray terrain_recv_array;
	//TArray<int8> terrain_recv_array;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FOneArray> terrain_2d_array;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> TerrainBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray< AActor*> TerrainBlocks;
	



	/*DWORD WINAPI Angle_Receiver(LPVOID arg);*/
};

