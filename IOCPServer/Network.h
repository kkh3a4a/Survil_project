#pragma once
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <array>
#include "protocol.h"
#include"Object.h"

extern std::array<class Object*, MAXOBJECT> objects;
extern HANDLE h_iocp;


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
	char	_buf[BUFSIZE];

public:
	WSA_OVER_EX();
	WSA_OVER_EX(IOCPOP iocpop, char byte, void* buf);
	WSAOVERLAPPED& getWsaOver() { return _wsaover; };
	char* getbuf() { return _buf; };

	void processpacket(int client_id, char* packet);
	void send_login_ok_packet(int client_id);
	void set_accept_over();
};


