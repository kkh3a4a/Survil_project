// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Sockets.h"
#include "MeshTerrain.h"
#include "SocketSubsystem.h"
#include <WS2tcpip.h>

#pragma comment (lib, "WS2_32.LIB")

class PROJECT_API NetworkingThread
{
public:
	NetworkingThread();
	~NetworkingThread();
};

class FSocketThread : public FRunnable
{
public:
    FSocketThread(AActor*);

    void Stop();
    virtual uint32_t Run() override;

    bool IsRunning = true;
	bool IsConnected = false;
    double CycleTime{};

private: 
   // FString IPAddress = "192.168.0.8";
    char IPAddress[20] = "127.0.0.1";
    int32 SERVER_PORT = 9000;
    SOCKET s_socket;
    int32 BytesReceived;
    int32 BytesSent;
    
    void error_display(const char* msg, int err_no);

};