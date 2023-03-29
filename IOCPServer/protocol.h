#pragma once


#define BUFSIZE			255
#define SERVERPORT		9000

#define MAXNAMESIZE		20
#define MAXOBJECT		5000

#define MAXPLAYER		5
#define CITIZENSTART	MAXPLAYER
#define MAXCITIZEN		200



#define CS_PACKET_LOGIN			1
#define CS_PACKET_MOVE			2
#define CS_PACKET_CITIZENCREATE	3
#define CS_PACKET_CITIZENMOVE	4

#define SC_PACKET_LOGIN			1
#define SC_PACKET_MOVE			2
#define SC_PACKET_CITIZENCREATE	3
#define SC_PACKET_CITIZENMOVE	4

#pragma pack (push, 1)
struct cs_packet_login
{
	unsigned char size;
	unsigned char type;
};

struct cs_packet_move
{
	unsigned char size;
	unsigned char type;
	char w, a, s, d;
};

struct cs_packet_citizencreate
{
	unsigned char size;
	unsigned char type;
	//추후 서버에 citizen 생성할때 쓰일 예정
};

struct cs_packet_citizenmove
{
	unsigned char size;
	unsigned char type;
	int _citizenid;
	float x, y, z;
};








struct sc_packet_login
{
	unsigned char size;
	unsigned char type;
	float x, y, z;
	float rx, ry, rz;
	char player_id;
};

struct sc_packet_move
{
	unsigned char size;
	unsigned char type;
	float currentX, currentY, currentZ;
	float currentRX, currentRY, currentRZ;
};

struct sc_packet_citizencreate
{
	unsigned char size;
	unsigned char type;
	int _citizenid;
	float x, y, z;
};


struct sc_packet_citizenmove
{
	unsigned char size;
	unsigned char type;
	int _citizenid;
	float x, y, z;
	float rx, ry, rz;
};










#pragma pack (pop)