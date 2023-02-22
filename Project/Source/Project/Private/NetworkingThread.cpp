// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkingThread.h"

NetworkingThread::NetworkingThread()
{
	
}

NetworkingThread::~NetworkingThread()
{
	
}

FSocketThread::FSocketThread(const FString& InIPAddress, const int32 InPortNumber)
    : IPAddress(InIPAddress)
    , PortNumber(InPortNumber)
    , Socket(nullptr)
    , IsConnected(false)
	, Running(false)
{}

bool FSocketThread::Init()
{
    // Create a socket object
    Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

    // Connect to the server
    TSharedPtr<FInternetAddr> ServerAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    bool bIsValid;
    ServerAddress->SetIp(*IPAddress, bIsValid);
    ServerAddress->SetPort(PortNumber);
    IsConnected = Socket->Connect(*ServerAddress);
    UE_LOG(LogTemp, Warning, TEXT("Message from separate thread Connected to Server!"));

    return IsConnected;
}

uint32 FSocketThread::Run()
{

    while (IsConnected && Running)
    {
        // Receive data from the server
        int32 BytesReceived = 0;
        //IsConnected = Socket->Recv((uint8*)&Buffer, sizeof(Buffer), BytesReceived);
		//IsConnected = Socket->Send((uint8*)&Buffer, sizeof(Buffer), BytesReceived);
		
		

		IsConnected = Socket->Send((uint8*)&Citizen_moving, sizeof(FCitizen_moving), BytesReceived);
		//ret = send(s_socket, (char*)&Citizen_moving, (int)sizeof(FCitizen_moving), 0);
		IsConnected = Socket->Recv((uint8*)&sunangle, sizeof(Fthree_float), BytesReceived);
		//ret = recv(s_socket, (char*)&sunangle, (int)sizeof(Fthree_float), 0);


		for (int i = 0; i < MAXPLAYER; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				IsConnected = Socket->Recv((uint8*)&temp_Actor, sizeof(FCitizen_sole), BytesReceived);
				//recv(s_socket, (char*)&temp_Actor, sizeof(FCitizen_sole), 0);

				TF_set(citizen[i].citizen_location_rotation[j].location, temp_Actor.location);

			}
		}


		for (int i = 0; i < MAXPLAYER * 10; ++i)
		{
			IsConnected = Socket->Recv((uint8*)&temp_resource, sizeof(Fresources_actor), BytesReceived);
			//recv(s_socket, (char*)&temp_resource, sizeof(Fresources_actor), 0);
			resoure_set(resources_create_landscape[i], temp_resource);
		}

		//카메라 위치 및 입력보내버리기
		IsConnected = Socket->Send((uint8*)&my_key_input, sizeof(Fkeyboard_input), BytesReceived);
		//send(s_socket, (char*)&my_key_input, sizeof(Fkeyboard_input), 0);
		IsConnected = Socket->Recv((uint8*)&my_camera_location, sizeof(Fthree_float), BytesReceived);
		//recv(s_socket, (char*)&my_camera_location, sizeof(Fthree_float), 0);


		//자원 받기
		IsConnected = Socket->Recv((uint8*)&resources, sizeof(int)*5, BytesReceived);
		//recv(s_socket, (char*)&resources, sizeof(int) * 5, 0);
		oil_count = resources[0], water_count = resources[1], iron_count = resources[2], food_count = resources[3], wood_count = resources[4];


		//Recv Terrain
		for (int i = 0; i < map_size; i++) {
			IsConnected = Socket->Recv((uint8*)&terrain_recv_array, (sizeof(char) * map_size), BytesReceived);
			//ret = recv(s_socket, (char*)&terrain_recv_array, (int)(sizeof(char) * map_size), 0);
			for (int j = 0; j < map_size; j++)
			{
				Terrain2DArray[i][j] = terrain_recv_array[j];
			}
		}
    }
    return 0;
}

void FSocketThread::Stop()
{
    // Close the socket connection
    if (Socket)
    {
        Socket->Close();
        delete Socket;
        Socket = nullptr;
    }
}

FSocket* FSocketThread::GetSocket()
{
	return Socket;
}


void FSocketThread::citizen_set(int i, int j)
{
	FCitizen_sole citizentemp;
	TF_set(citizentemp.location, temp_Actor.location);
	citizen[i].citizen_location_rotation.Add(citizentemp);
	TF_set(citizen[i].citizen_location_rotation[j].location, temp_Actor.location);
}

void FSocketThread::TF_set(Fthree_float& a, Fthree_float& b)
{
	a.x = b.x;
	a.y = b.y;
	a.z = b.z;
}

void FSocketThread::resoure_set(Fresources_actor& a, Fresources_actor& b)
{
	a.count = b.count;
	a.type = b.type;
	TF_set(a.location, b.location);
}
