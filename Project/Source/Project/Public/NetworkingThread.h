// Fill out your copyright notice in the Description page of Project Settings.

#pragma once




#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Sockets.h"
#include "MeshTerrain.h"

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
    FSocketThread(AActor* temp_server_testing);

    void ServerGetter();
    //virtual bool Init() override;
    virtual uint32_t Run() override;
    //virtual void Stop() override;
    //AServer_testing* server_testing;

private: 
       
};