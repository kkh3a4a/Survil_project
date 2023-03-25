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
	WSADATA WSAData;
	int ret = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (ret != 0)
	{
		exit(-1);
	}
	s_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, IPAddress, &server_addr.sin_addr);
	ret = connect(s_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	if (ret != 0)
	{
		exit(-1);
	}
}

uint32_t FSocketThread::Run()
{
	if (!IsRunning || !IsConnected)
		return 0;

	//Recv Struct
	IsConnected = Socket->Recv((uint8*)&MainClass->ServerStruct1, sizeof(MainClass->ServerStruct1), BytesReceived, ESocketReceiveFlags::WaitAll);
	if (BytesReceived != sizeof(MainClass->ServerStruct1)) {
		UE_LOG(LogTemp, Warning, TEXT("Network Recv Error!!"));
		IsConnected = false;
	}
	IsConnected = Socket->Recv((uint8*)&MainClass->ServerStruct2, sizeof(MainClass->ServerStruct2), BytesReceived, ESocketReceiveFlags::WaitAll);
	if (!IsConnected) {
		UE_LOG(LogTemp, Warning, TEXT("Network Recv Error!!"));
		IsConnected = false;
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

	while (IsConnected && IsRunning) {
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
				MainClass->CycleNum = 0;
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
			

			//º¸³Â´ÂÁö ¾Èº¸³Â´ÂÁö È®ÀÎÇØÁà¾ßÇÔ
			if (MainClass->RecvedUIInput == false) {
				if (MainClass->UI_Input.ResourceInput.CitizenCountAdd || MainClass->UI_Input.ResourceInput.CitizenCountSub) {
					{
						MainClass->RecvedUIInput = true;
					}
				}
			}
			else if (MainClass->RecvedUIInput == true) {
				if (MainClass->CitizenRelaese)
				{
					MainClass->RecvedUIInput = false;
				}
				MainClass->UI_Input.ResourceInput.CitizenCountAdd = false;
				MainClass->UI_Input.ResourceInput.CitizenCountSub = false;
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

void FSocketThread::error_display(const char* msg, int err_no)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	LocalFree(lpMsgBuf);

}

void FSocketThread::Stop()
{
	IsRunning = false;
}
