// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include<WS2tcpip.h>
#include "HAL/Runnable.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "global.h"
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
    FSocketThread(const FString& InIPAddress, const int32 InPortNumber);

    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    FSocket* GetSocket();
    FThreadSafeBool IsConnected;

    //Networking Data
    FCitizen_moving Citizen_moving;
    Fthree_float sunangle;
    FCitizen_sole temp_Actor;
    Fresources_actor temp_resource;
    Fkeyboard_input my_key_input;
    Fthree_float my_camera_location;
    int resources[5] = {};
    int oil_count = 0;
    int water_count = 0;
    int iron_count = 0;
    int food_count = 0;
    int wood_count = 0;
    int8 terrain_recv_array[MapSizeX];
    TArray<TArray<int8>> Terrain2DArray;
    AMeshTerrain* TerrainActor;

    void citizen_set(int i, int j);
    void TF_set(Fthree_float& a, Fthree_float& b);
    void resoure_set(Fresources_actor& a, Fresources_actor& b);
    TMap<int, Fcitizen_struct> citizen;
    TMap<int, Fresources_actor> resources_create_landscape;
    
private:
    FString IPAddress;
    int32 PortNumber;
    FSocket* Socket;
    bool Running;
    


    

    
};