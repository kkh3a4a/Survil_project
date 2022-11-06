// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include<WS2tcpip.h>
#include<iostream>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Server_testing.generated.h"

using namespace std;

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

};
