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
	IsConnected = Socket->Connect(*ServerAddress);
	if (IsRunning) {
		UE_LOG(LogTemp, Warning, TEXT("Connected to Server!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Not Connected to Server!"));
	}
}

uint32_t FSocketThread::Run()
{
	if (!IsRunning)
		return 0;

	//Recv Struct
	IsRunning = Socket->Recv((uint8*)&MainClass->ServerStruct1, sizeof(MainClass->ServerStruct1), BytesReceived, ESocketReceiveFlags::WaitAll);
	if (BytesReceived != sizeof(MainClass->ServerStruct1)) {
		UE_LOG(LogTemp, Warning, TEXT("Network Recv Error!!"));
		IsRunning = false;
	}
	IsConnected = Socket->Recv((uint8*)&MainClass->ServerStruct2, sizeof(MainClass->ServerStruct2), BytesReceived, ESocketReceiveFlags::WaitAll);
	if (!IsConnected) {
		UE_LOG(LogTemp, Warning, TEXT("Network Recv Error!!"));
		IsConnected = false;
		return 0;
	}
	for (int thread_cnt_num = 0; thread_cnt_num < MAXPLAYER; ++thread_cnt_num) {
		MainClass->players_list.Add(thread_cnt_num, &(MainClass->ServerStruct1.PlayerInfo));
	}
	MainClass->Citizens->Citizen_moving->Team = -1;
	MainClass->Citizens->Citizen_moving->CitizenNumber = -1;

	memcpy(&MainClass->ClientStruct1.MyCitizenMoving, MainClass->Citizens->Citizen_moving, sizeof(FCitizenMoving));
	MainClass->Citizens->Citizen_moving = &MainClass->ClientStruct1.MyCitizenMoving;
	MainClass->ThreadInitSendRecv = true;

	steady_clock::time_point start_t = high_resolution_clock::now();
	while (IsRunning) {
		steady_clock::time_point end_t = high_resolution_clock::now();
		CycleTime = duration_cast<milliseconds>(end_t - start_t).count();

		if (CycleTime > 50 && IsConnected){
			start_t = high_resolution_clock::now();

			//Recv Struct
			if (IsConnected) {
				IsConnected = Socket->Recv((uint8*)&MainClass->ServerStruct1, sizeof(MainClass->ServerStruct1), BytesReceived, ESocketReceiveFlags::WaitAll);
				if (!IsConnected) {
					UE_LOG(LogTemp, Warning, TEXT("Network Recv Error!!"));
					IsConnected = false;
					return 0;
				}
			}

			if (IsConnected) {
				IsConnected = Socket->Recv((uint8*)&MainClass->ServerStruct2, sizeof(MainClass->ServerStruct2), BytesReceived, ESocketReceiveFlags::WaitAll);
				if (!IsConnected) {
					UE_LOG(LogTemp, Warning, TEXT("Network Recv Error!!"));
					IsConnected = false;
					return 0;
				}
			}
			//Recv Terrain
			if (IsConnected) {
				for (int thread_cnt_num = 0; thread_cnt_num < MapSizeX; thread_cnt_num++) {
					IsConnected = Socket->Recv((uint8*)&MainClass->Terrain2DArray[thread_cnt_num], sizeof(char) * MapSizeY, BytesReceived, ESocketReceiveFlags::WaitAll);
					if (!IsConnected) {
						UE_LOG(LogTemp, Warning, TEXT("Network Recv Error!!"));
						IsConnected = false;
						return 0;
					}
				}
			}
			//Recv Temperature
			if (IsConnected) {
				for (int thread_cnt_num = 0; thread_cnt_num < MapSizeX; thread_cnt_num++) {
					IsConnected = Socket->Recv((uint8*)&MainClass->TerrainTemperature[thread_cnt_num], sizeof(char) * MapSizeY, BytesReceived, ESocketReceiveFlags::WaitAll);
					if (!IsConnected) {
						UE_LOG(LogTemp, Warning, TEXT("Network Recv Error!!"));
						IsConnected = false;
						return 0;
					}
				}
			}
			//Send Struct
			if (IsConnected) {
				IsConnected = Socket->Send((uint8*)&MainClass->ClientStruct1, sizeof(FClientStruct1), BytesSent);
			}
		}
		else{
			Sleep(1);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Network Thread End!!"));
	MainClass->ClientStruct1.connecting = -1;
	IsConnected = Socket->Send((uint8*)&MainClass->ClientStruct1, sizeof(FClientStruct1), BytesSent);
	return 0;
}

void FSocketThread::Stop()
{
	IsRunning = false;
}
