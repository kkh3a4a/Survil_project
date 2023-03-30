// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkingThread.h"
#include"Main.h"
#include"Citizen.h"
#include "MyPlayerController.h"
#include "CitizenManager.h"
#include "ResourceManager.h"
#include "Kismet/GameplayStatics.h"
using namespace std;


FSocketThread* fsocket_thread;

NetworkingThread::NetworkingThread()
{
	
}

NetworkingThread::~NetworkingThread()
{
	
}


WSA_OVER_EX::WSA_OVER_EX()
{
	//exit(-1);
	return;
}

WSA_OVER_EX::WSA_OVER_EX(IOCPOP iocpop, char byte, void* buf)
{
	ZeroMemory(&_wsaover, sizeof(_wsaover));
	_iocpop = iocpop;
	_wsabuf.buf = reinterpret_cast<char*>(buf);
	_wsabuf.len = byte;
}





FSocketThread::FSocketThread()
{
	IsRunning = true;
	fsocket_thread = this;
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
	server_addr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, IPAddress, &server_addr.sin_addr);
	ret = connect(s_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	if (ret != 0)
	{
		exit(-1);
	}
	UE_LOG(LogTemp, Warning, TEXT("Network Thread connect!!"));

	cs_packet_login packet_login;
	packet_login.size = sizeof(packet_login);
	packet_login.type = CS_PACKET_LOGIN;
	WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(IOCPOP::OP_SEND, sizeof(cs_packet_login), &packet_login);

	ret = WSASend(s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
	if (ret != 0)
	{
		//exit(-1);
	}
	DWORD r_flags = 0;
	ZeroMemory(&_recv_over_ex, sizeof(_recv_over_ex));
	_recv_over_ex._wsabuf.buf = reinterpret_cast<char*>(_recv_over_ex._buf);
	_recv_over_ex._wsabuf.len = sizeof(_recv_over_ex._buf);
	ret = WSARecv(s_socket, &_recv_over_ex._wsabuf, 1, 0, &r_flags, &_recv_over_ex._wsaover, recv_callback);
	if (ret != 0)
	{
		//exit(-1);
	}
}

uint32_t FSocketThread::Run()
{
	while (IsRunning)
	{
		SleepEx(10, true);
	}
	UE_LOG(LogTemp, Warning, TEXT("Network Thread End!!"));
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

void FSocketThread::processpacket(unsigned char* buf)
{
	unsigned char packet_type = buf[1];
	switch (packet_type)
	{
	case SC_PACKET_LOGIN:
	{
		sc_packet_login* packet = reinterpret_cast<sc_packet_login*>(buf);
		fsocket_thread->_MainClass->SetPlayerLocation(packet->x, packet->y, packet->z);
		fsocket_thread->_MainClass->SetCurrentLocation(- MapSizeX * 100 / 2, - MapSizeY * 100 / 2, packet->z);
		fsocket_thread->my_id = packet->player_id;
		break;
	}
	case SC_PACKET_MOVE:
	{
		sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(buf);
		fsocket_thread->_MainClass->SetCurrentLocation(packet->currentX, packet->currentY, packet->currentZ);
		//UE_LOG(LogTemp, Warning, TEXT("current Location : %f %f %f"), packet->currentX, packet->currentY, packet->currentZ);
		break;
	}
	case SC_PACKET_CITIZENCREATE:
	{
		sc_packet_citizencreate* packet = reinterpret_cast<sc_packet_citizencreate*>(buf);
		
		_CitizenManager->Spawn_Citizen(packet->_citizenid - CITIZENSTART, FVector(packet->x , packet->y, packet->z));
		
		break;
	}
	case SC_PACKET_CITIZENMOVE:
	{
		sc_packet_citizenmove* packet = reinterpret_cast<sc_packet_citizenmove*>(buf);
		FRotator Rotation = (FVector(packet->rx, packet->ry, packet->rz)).GetSafeNormal().Rotation();
		_CitizenManager->Set_Citizen_Location(packet->_citizenid - CITIZENSTART, FVector(packet->x, packet->y, packet->z), Rotation);
		//UE_LOG(LogTemp, Warning, TEXT("Rotate : %f %f %f"), Rotation.Pitch, Rotation.Yaw, Rotation.Roll);
		break;
	}
	case SC_PACKET_RESOURCECREATE:
	{
		sc_packet_resourcecreate* packet = reinterpret_cast<sc_packet_resourcecreate*>(buf);
		_ResourceManager->Spawn_Resource(packet->_resourceid - RESOURCESTART, FVector(packet->x, packet->y, packet->z), packet->_amount, packet->resource_type);
		break;
	}


	default:
	{
		//DebugBreak();
		break;
	}
	}
}

void FSocketThread::Stop()
{
	IsRunning = false;
}





void CALLBACK send_callback(DWORD err, DWORD num_byte, LPWSAOVERLAPPED send_over, DWORD flag)
{
	WSA_OVER_EX* wsa_over_ex = reinterpret_cast<WSA_OVER_EX*>(send_over);
	delete  wsa_over_ex;
}


void CALLBACK recv_callback(DWORD err, DWORD num_byte, LPWSAOVERLAPPED recv_over, DWORD flag) 
{
	if (err != 0)
	{
		return;
	}
	WSA_OVER_EX* wsa_over_ex = reinterpret_cast<WSA_OVER_EX*>(recv_over);
	
	//여기서 패킷 재조립을 해준다.
	unsigned char* packet_start = wsa_over_ex->_buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static unsigned char packet_buffer[BUFSIZE];

	while (0 != num_byte) {
		if (0 == in_packet_size) in_packet_size = packet_start[0];
		if (num_byte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, packet_start, in_packet_size - saved_packet_size);

			fsocket_thread->processpacket(packet_buffer);
			packet_start += in_packet_size - saved_packet_size;
			num_byte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, packet_start, num_byte);
			saved_packet_size += num_byte;
			num_byte = 0;
		}
	}

	ZeroMemory(&wsa_over_ex->_wsaover, sizeof(wsa_over_ex->_wsaover));
	DWORD r_flags = 0;
	WSARecv(fsocket_thread->s_socket, &fsocket_thread->_recv_over_ex._wsabuf, 1, 0, &r_flags, &fsocket_thread->_recv_over_ex._wsaover, recv_callback);

}