// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include<WS2tcpip.h>
#include "Sockets.h"
#include "SocketSubsystem.h"

/**
 * 
 */
class PROJECT_API NetworkingThread
{
public:
	NetworkingThread();
	~NetworkingThread();
};

class FSocketThread : public FRunnable
{
public:
    FSocketThread(const FString& InIPAddress, const int32 InPortNumber);

    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;

    FThreadSafeBool IsConnected;

private:
    FString IPAddress;
    int32 PortNumber;
    FSocket* Socket;
};