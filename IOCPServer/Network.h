#pragma once
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <array>
#include "protocol.h"
#include"Object.h"
#include<set>
#include<mutex>


extern std::array<class Object*, MAXOBJECT> objects;
extern HANDLE h_iocp;
extern bool IsNight;
extern char** object_z;
extern int survil_day;
extern std::set<int> sand_storm_day;
bool CAS(volatile int* addr, int expected, int update);
extern bool Is_sand_storm;
extern volatile int room_player_cnt;
enum IOCPOP
{
	OP_RECV,
	OP_SEND,
	OP_ACCEPT
};
enum class STATE { ST_FREE, ST_CONNECT, ST_INGAME, ST_DISCONNECT };

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


void all_player_sendpacket(void* packet);

bool overlap_check(int id1, int id2, float range);

bool object_find_check(int id1, int id2, float range);