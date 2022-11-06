// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Windows/AllowWindowsPlatformTypes.h"

// include header with included Windows.h


#include<WS2tcpip.h>
#include<iostream>

#include "Windows/HideWindowsPlatformTypes.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Server_testing.generated.h"

using namespace std;

USTRUCT(Atomic, BlueprintType)
struct FSunAngle {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float x = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float y = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float z = 0;
};

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
	WSADATA WSAData;
	SOCKET s_socket;
	HANDLE hThread;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FSunAngle sunangle;

	


	/*
	DWORD WINAPI Angle_Receiver(LPVOID arg);*/
};

