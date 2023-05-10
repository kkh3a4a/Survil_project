// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include<iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include"../../../../IOCPServer/protocol.h"
#include "HAL/Runnable.h"
#include "MeshTerrain.h"
#include "CoreMinimal.h"

#pragma comment (lib,"MSWSock.lib")
#pragma comment (lib, "WS2_32.LIB")
void CALLBACK send_callback(DWORD err, DWORD num_byte, LPWSAOVERLAPPED send_over, DWORD flag);
void CALLBACK recv_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED recv_over, DWORD flag);

enum IOCPOP
{
	OP_RECV,
	OP_SEND,
	OP_ACCEPT
};

class PROJECT_API NetworkingThread
{
public:
	NetworkingThread();
	~NetworkingThread();
};

class WSA_OVER_EX {
public:
	WSAOVERLAPPED	_wsaover;
	IOCPOP			_iocpop;
	WSABUF			_wsabuf;
	unsigned char	_buf[BUFSIZE];

public:
	WSA_OVER_EX();
	WSA_OVER_EX(IOCPOP iocpop, char byte, void* buf);
	WSAOVERLAPPED& getWsaOver() { return _wsaover; };
};

class FSocketThread : public FRunnable
{
public:
    FSocketThread();

    void Stop();
    virtual uint32_t Run() override;

	class AMain* _MainClass;
	class AMyPlayerController* _MyController;
	class ACitizenManager* _CitizenManager;
	class AResourceManager* _ResourceManager;
	class ABuildManager* _BuildManager;

    bool IsRunning = true;
	bool IsConnected = false;
    double CycleTime{};
	WSA_OVER_EX _recv_over_ex;
	SOCKET s_socket;

	int _prev_size = 0;
	char playerjobless = { FIRSTCITIZENCREATE };

	char my_id;
	void processpacket(unsigned char* buf);
	void error_display(const char* msg, int err_no);

private: 
	//char IPAddress[20] = "192.168.0.8";
	//char IPAddress[20] = "127.0.0.1";
	char IPAddress[20] = "172.20.10.2";
    int32 BytesReceived;
    int32 BytesSent;

};