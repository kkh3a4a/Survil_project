// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkingThread.h"
#include "Main.h"
#include "Kismet/GameplayStatics.h"

using namespace chrono;
using namespace std;
AMain* MainClass;

NetworkingThread::NetworkingThread()
{
	
}

NetworkingThread::~NetworkingThread()
{
	
}

FSocketThread::FSocketThread(AActor* main)
{
	MainClass = Cast<AMain>(main);
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	// Connect to the server
	TSharedPtr<FInternetAddr> ServerAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool bIsValid;
	ServerAddress->SetIp(*IPAddress, bIsValid);
	ServerAddress->SetPort(PortNumber);
	IsConnected = Socket->Connect(*ServerAddress);
	if (IsConnected) {
		UE_LOG(LogTemp, Warning, TEXT("Connected to Server!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Not Connected to Server!"));
	}
}

uint32_t FSocketThread::Run()
{
	if (!IsRunning || !IsConnected)
		return 0;

	//버퍼 사이즈와 버퍼 지정
	const int RecvBufferSize = sizeof(float) + sizeof(FTwoInt) + (sizeof(char) + sizeof(FTwoInt) + sizeof(FCitizenSole) * 100 + sizeof(FResource) * 5 + sizeof(FBuildingInfo) * 20) * MAXPLAYER;
	char* RecvBuffer = new char[RecvBufferSize];
	const int SendBufferSize = sizeof(FKeyInput) + sizeof(FUI_Input);
	char* SendBuffer = new char[SendBufferSize];
	UE_LOG(LogTemp, Warning, TEXT("Recv Buffer Size: %d, Send Buffer Size: %d"), RecvBufferSize, SendBufferSize);

	for (int i = 0; i < MAXPLAYER; i++) {
		FPlayerInfo* player = new FPlayerInfo;
		MainClass->Players.Add(i, player);
	}
	MainClass->Citizens->Citizen_moving->Team = -1;
	MainClass->Citizens->Citizen_moving->CitizenNumber = -1;

	steady_clock::time_point start_t = high_resolution_clock::now();

	while (IsConnected && IsRunning) {
		steady_clock::time_point end_t = high_resolution_clock::now();
		CycleTime = duration_cast<milliseconds>(end_t - start_t).count();
		if (CycleTime > 50 && IsConnected){
			start_t = high_resolution_clock::now();

			if (IsConnected) {
				ZeroMemory(RecvBuffer, RecvBufferSize);
				IsConnected = Socket->Recv((uint8*)RecvBuffer, RecvBufferSize, BytesReceived, ESocketReceiveFlags::WaitAll);
				if (!IsConnected) {
					UE_LOG(LogTemp, Warning, TEXT("Network Recv Error!!"));
					IsConnected = false;
				}
				// 받은 패킷 뜯기
				char Identity{};
				int CopyAddress{};
				memcpy(&MainClass->SunAngle, RecvBuffer + CopyAddress, sizeof(float));
				CopyAddress += sizeof(float);
				memcpy(&MainClass->CurrentLocation, RecvBuffer + CopyAddress, sizeof(FTwoInt));
				CopyAddress += sizeof(FTwoInt);
				for (int i = 0; i < MAXPLAYER; i++) {
					memcpy(&Identity, RecvBuffer + CopyAddress, sizeof(char));
					CopyAddress += sizeof(char);
					memcpy(&MainClass->Players[i]->CityLocation, RecvBuffer + CopyAddress, sizeof(FTwoInt));
					CopyAddress += sizeof(FTwoInt);
					memcpy(&MainClass->Players[i]->Citizen, RecvBuffer + CopyAddress, sizeof(FCitizenSole) * 100);
					CopyAddress += sizeof(FCitizenSole) * 100;
					memcpy(&MainClass->Players[i]->Resource, RecvBuffer + CopyAddress, sizeof(FResource) * 5);
					CopyAddress += sizeof(FResource) * 5;
					memcpy(&MainClass->Players[i]->Building, RecvBuffer + CopyAddress, sizeof(FBuildingInfo) * 20);
					CopyAddress += sizeof(FBuildingInfo) * 20;
				}
				if (CopyAddress != RecvBufferSize)
					UE_LOG(LogTemp, Warning, TEXT("FATAL ERROR!!!! RECV FAIL %d, %d"), CopyAddress, RecvBufferSize);
			}

			MainClass->ThreadInitSendRecv = true;
			MainClass->CycleNum = 0;

			//Recv Terrain
			if (IsConnected) {
				for (int i = 0; i < MapSizeX; i++) {
					IsConnected = Socket->Recv((uint8*)&MainClass->Terrain2DArray[i], sizeof(char) * MapSizeY, BytesReceived, ESocketReceiveFlags::WaitAll);
					if (!IsConnected) {
						UE_LOG(LogTemp, Warning, TEXT("Network Recv Error!!"));
						IsConnected = false;
						return 0;
					}
				}
			}

			//Recv Temperature
			if (IsConnected) {
				for (int i = 0; i < MapSizeX / 2; i++) {
					IsConnected = Socket->Recv((uint8*)&MainClass->TerrainTemperature[i], sizeof(char) * MapSizeY / 2, BytesReceived, ESocketReceiveFlags::WaitAll);
					if (!IsConnected) {
						UE_LOG(LogTemp, Warning, TEXT("Network Recv Error!!"));
						IsConnected = false;
						return 0;
					}
				}
			}
			
			//보냈는지 안보냈는지 확인해줘야함
			if (MainClass->RecvedUIInput == false) {
				if (MainClass->UI_Input.ResourceInput.CitizenCountAdd || MainClass->UI_Input.ResourceInput.CitizenCountSub) {
					{
						MainClass->RecvedUIInput = true;
					}
				}
			}
			else if (MainClass->RecvedUIInput == true) {
				if (MainClass->CitizenRelaese) {
					MainClass->RecvedUIInput = false;
				}
				MainClass->UI_Input.ResourceInput.CitizenCountAdd = false;
				MainClass->UI_Input.ResourceInput.CitizenCountSub = false;
			}

			//Send Struct
			if (IsConnected) {
				//보낼 패킷 포장
				ZeroMemory(SendBuffer, SendBufferSize);
				memcpy(SendBuffer, &MainClass->UI_Input, sizeof(FUI_Input));
				memcpy(SendBuffer + sizeof(FUI_Input), &MainClass->KeyInput, sizeof(FKeyInput));
				IsConnected = Socket->Send((uint8*)SendBuffer, SendBufferSize, BytesSent);
				if (BytesSent != SendBufferSize) {
					UE_LOG(LogTemp, Warning, TEXT("send Fatal error"));
					IsConnected = false;
					return 0; 
				}
			}
		}
		else{
			Sleep(1);
		}
	}
	delete SendBuffer;
	delete RecvBuffer;
	UE_LOG(LogTemp, Warning, TEXT("Network Thread End!!"));
	return 0;
}

void FSocketThread::Stop()
{
	IsRunning = false;
	IsConnected = false;
}
