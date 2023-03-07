// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkingThread.h"
#include "Server_testing.h"
#include "Kismet/GameplayStatics.h"

using namespace chrono;
using namespace std;
AServer_testing* MainClass;

NetworkingThread::NetworkingThread()
{
	
}

NetworkingThread::~NetworkingThread()
{
	
}

FSocketThread::FSocketThread(AActor* temp_server_testing)
{
	MainClass = Cast<AServer_testing>(temp_server_testing);
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	// Connect to the server
	TSharedPtr<FInternetAddr> ServerAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool bIsValid;
	ServerAddress->SetIp(*IPAddress, bIsValid);
	ServerAddress->SetPort(PortNumber);
	IsRunning = Socket->Connect(*ServerAddress);
	if(IsRunning)
		UE_LOG(LogTemp, Warning, TEXT("Connected to Server!"));
}

uint32_t FSocketThread::Run()
{
	if (!IsRunning)
		return 0;

	//Recv Struct
	IsRunning = Socket->Recv((uint8*)&MainClass->ServerSendStruct, sizeof(MainClass->ServerSendStruct), BytesReceived);
	for (int thread_cnt_num = 0; thread_cnt_num < MAXPLAYER; ++thread_cnt_num) {
		MainClass->players_list.Add(thread_cnt_num, &(MainClass->ServerSendStruct.player_info));
	}
	MainClass->first_recv_send = true;
	MainClass->Citizens->Citizen_moving->team = -1;
	MainClass->Citizens->Citizen_moving->citizen_number = -1;

	memcpy(&MainClass->ClientSendStruct.My_citizen_moving, MainClass->Citizens->Citizen_moving, sizeof(FCitizen_moving));
	MainClass->Citizens->Citizen_moving = &MainClass->ClientSendStruct.My_citizen_moving;
	MainClass->Isthreading_first_send = true;

	steady_clock::time_point start_t = high_resolution_clock::now();
	while(IsRunning){
		steady_clock::time_point end_t = high_resolution_clock::now();
		if (duration_cast<milliseconds>(end_t - start_t).count() > 50){
			start_t = high_resolution_clock::now();
			MainClass->temped += 1;
			IsRunning = Socket->Recv((uint8*)&MainClass->ServerSendStruct, sizeof((MainClass->ServerSendStruct)), BytesReceived);

			//Recv Terrain
			for (int thread_cnt_num = 0; thread_cnt_num < MapSizeX; thread_cnt_num++) {
				IsRunning = Socket->Recv((uint8*)&MainClass->Terrain2DArray[thread_cnt_num], sizeof(char) * MapSizeY, BytesReceived);
			}

			//Recv Temperature
			for (int thread_cnt_num = 0; thread_cnt_num < MapSizeX; thread_cnt_num++) {
				IsRunning = Socket->Recv((uint8*)&MainClass->TerrainTemperature[thread_cnt_num], sizeof(char) * MapSizeY, BytesReceived);
			}

			//Send Struct
			IsRunning = Socket->Send((uint8*)&MainClass->ClientSendStruct, sizeof(FClientSendInfo), BytesReceived);
		}
		else{
			Sleep(1);
		}
	}
	return 0;
}

void FSocketThread::Stop()
{
	IsRunning = false;
}
