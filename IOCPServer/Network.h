#pragma once
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <array>
#include "protocol.h"
#include"Object.h"

extern std::array<class Object*, MAXOBJECT> objects;
extern HANDLE h_iocp;
extern bool IsNight;

enum IOCPOP
{
	OP_RECV,
	OP_SEND,
	OP_ACCEPT
};

class WSA_OVER_EX {
public:
	WSAOVERLAPPED	_wsaover;
	IOCPOP			_iocpop;
	WSABUF			_wsabuf;
	unsigned char	_buf[BUFSIZE];

public:
	WSA_OVER_EX();
	WSA_OVER_EX(IOCPOP iocpop, unsigned char byte, void* buf);
	WSAOVERLAPPED& getWsaOver() { return _wsaover; };
	unsigned char* getbuf() { return _buf; };

	void processpacket(int client_id, unsigned char* packet);
	void send_login_ok_packet(int client_id);
	void send_citizen_First_create_packet(int client_id);
	void send_resource_First_create_packet(int client_id);

	void set_accept_over();
};

