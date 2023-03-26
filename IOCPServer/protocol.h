#pragma once


#define BUFSIZE			255
#define SERVERPORT		9000

#define MAXNAMESIZE		20
#define MAXOBJECT		5000

#define MAXPLAYER		5




#define CS_PACKET_LOGIN			1
#define CS_PACKET_MOVE			2

#define SC_PACKET_LOGIN			1
#define SC_PACKET_MOVE			2


struct cs_packet_login
{
	unsigned char size;
	unsigned char type;
	char name[MAXNAMESIZE];
};

struct cs_packet_PlayerKeyInput
{
	unsigned char size;
	unsigned char type;
	char w, a, s, d;
};








struct sc_packet_login
{
	unsigned char size;
	unsigned char type;
	float x, y, z;
	float rx, ry, rz;
	float cameraX, cameraY, cameraZ;
	float cameraRX, cameraRY, cameraRZ;
};



