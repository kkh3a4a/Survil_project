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
	const int BUFSIZE = 256;
	int ret = 0;
	TMap<FString, AActor*> My_Citizen;
	int My_Citizen_Num = 0;
	steady_clock::time_point start_t;
	WSADATA WSAData;
	SOCKET s_socket;
	HANDLE hThread;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FActor_location_rotation MouseInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Fthree_float sunangle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FActor_location_rotation test_Actor;

	char terrain_2d_array[64][64];

	/*
	DWORD WINAPI Angle_Receiver(LPVOID arg);*/
};

