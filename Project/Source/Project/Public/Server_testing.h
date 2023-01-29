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
const int map_size = 64;

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
	
	int8 get_height(int32, int32);

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, FActor_location_rotation> players_list;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, Fcitizen_struct> citizen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActor_location_rotation temp_Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, FAActor_struct> My_Citizen;

	int My_Citizen_Num = 0;

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
	Fthree_float sunangle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActor_location_rotation test_Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Citizen_num = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool first_recv_send = false;
	
	int8 terrain_recv_array[map_size];

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FOneArray> terrain_array;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 sands_size {map_size};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> int_array;


	/*DWORD WINAPI Angle_Receiver(LPVOID arg);*/
};

