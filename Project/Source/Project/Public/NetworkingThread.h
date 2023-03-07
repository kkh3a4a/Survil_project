// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Sockets.h"
#include "MeshTerrain.h"
#include "SocketSubsystem.h"
#include<WS2tcpip.h>

class PROJECT_API NetworkingThread
{
public:
	NetworkingThread();
	~NetworkingThread();
};

class FSocketThread : public FRunnable
{
public:
    FSocketThread(AActor* temp_server_testing);

    void Stop();
    virtual uint32_t Run() override;

    bool IsRunning = true;
	bool IsConnected = false;

private: 
    FString IPAddress = "192.168.0.8";
    int32 PortNumber = 9000;
    FSocket* Socket;
    int32 BytesReceived;
    int32 BytesSent;


};