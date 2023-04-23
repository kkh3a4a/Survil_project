#pragma once


#define BUFSIZE						512
#define SERVERPORT					9000

#define MAXNAMESIZE					20
#define MAXOBJECT					5000

#define MAXPLAYER					5
#define CITIZENSTART				MAXPLAYER
#define FIRSTCITIZENCREATE			20

#define PLAYERCITIZENCOUNT			200
#define MAXCITIZEN					1000
#define RESOURCESTART				(MAXCITIZEN + CITIZENSTART)
#define MAXRESOURCE					50
#define BUILDINGSTART				(MAXRESOURCE + RESOURCESTART)
#define PLAYERBUILDINGCOUNT			121
#define MAXBUILDING					(PLAYERBUILDINGCOUNT * MAXPLAYER)


#define SIGHT_X 200
#define SIGHT_Y 100


#define CS_PACKET_LOGIN				1
#define CS_PACKET_MOVE				2
#define CS_PACKET_CITIZENCREATE		3
#define CS_PACKET_CITIZENMOVE		4
#define CS_PACKET_RESOURCECREATE	5
#define CS_PACKET_SUNANGLE			6
#define CS_PACKET_CITIZENREMOVE		7

#define CS_PACKET_CITIZENPLACEMENT	10

#define CS_PACKET_RESOURCEAMOUNT	20
#define CS_PACKET_PLAYERRESOURCE	21

#define CS_PACKET_TERRAINALL		30
#define CS_PACKET_TERRAINXLOCATION	31
#define CS_PACKET_TERRAINYLOCATION	32

#define CS_PACKET_BUILDABLE			40
#define CS_PACKET_BUILD				41

#define CS_PACKET_TEMPERATURE		50



#define SC_PACKET_LOGIN				1
#define SC_PACKET_MOVE				2
#define SC_PACKET_CITIZENCREATE		3
#define SC_PACKET_CITIZENMOVE		4
#define SC_PACKET_RESOURCECREATE	5
#define SC_PACKET_SUNANGLE			6
#define SC_PACKET_CITIZENREMOVE		7

#define SC_PACKET_CITIZENPLACEMENT	10
#define SC_PACKET_RESOURCEAMOUNT	20
#define SC_PACKET_PLAYERRESOURCE	21

#define SC_PACKET_TERRAINALL		30
#define SC_PACKET_TERRAINXLOCATION	31
#define SC_PACKET_TERRAINYLOCATION	32

#define SC_PACKET_BUILDABLE			40
#define SC_PACKET_BUILD				41

#define SC_PACKET_TEMPERATURE		50
#define SC_PACKET_TEMPERATUREX		51
#define SC_PACKET_TEMPERATUREY		52


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
	//���� ������ citizen ���� ��û�Ҷ� ���� ����
};

struct cs_packet_citizenmove
{
	unsigned char size;
	unsigned char type;
	int citizenid;
	float x, y, z;
};

struct cs_packet_resourcecreate
{
	unsigned char size;	
	unsigned char type;
	
	//resource ���� ��û
};

struct cs_packet_citizenplacement
{
	unsigned char size;
	unsigned char type;
	int objectid;
	char isplus;

};

struct cs_packet_buildable
{
	unsigned char size = sizeof(cs_packet_buildable);
	unsigned char type = CS_PACKET_BUILDABLE;
	
	char building_type;
	float x, y;
};

struct cs_packet_build
{
	unsigned char size = sizeof(cs_packet_build);
	unsigned char type = CS_PACKET_BUILD;
	
	char building_type;
	float x, y;
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
	int citizenid;
	float x, y, z;
};

struct sc_packet_citizenremove
{
	unsigned char size;
	unsigned char type;
	int citizenid;
};

struct sc_packet_citizenplacement
{
	unsigned char size;
	unsigned char type;

	int object_id;
	char playerjobless;
	char workcitizen;
};

struct sc_packet_citizenmove
{
	unsigned char size;
	unsigned char type;
	int citizenid;
	float x, y, z;
	float rx, ry, rz;
};

struct sc_packet_resourcecreate
{
	unsigned char size;
	unsigned char type;

	int resource_id;
	float x, y, z;
	char resource_type;
	int amount;

};

struct sc_packet_resourceamount
{
	unsigned char size;
	unsigned char type;

	int resourceid;
	int amount;
};
struct sc_packet_playerresource
{
	unsigned char size;
	unsigned char type;

	int resources_acount[5];
};


struct sc_packet_terrainAll
{
	unsigned char size;
	unsigned char type;

	unsigned char terrain_X;
	char terrain_Y[SIGHT_Y];
};

struct sc_packet_terrainXlocation
{
	unsigned char size;
	unsigned char type;

	float terrainX;
	char terrainline_Y[SIGHT_Y];
};

struct sc_packet_terrainYlocation
{
	unsigned char size;
	unsigned char type;

	float terrainY;
	char terrainline_X[SIGHT_X];
};

struct sc_packet_sunangle
{
	unsigned char size;
	unsigned char type;

	float sunangle;
};

struct sc_packet_buildable
{
	unsigned char size = sizeof(sc_packet_buildable);
	unsigned char type = SC_PACKET_BUILDABLE;

	bool buildable;
};

struct sc_packet_build
{
	unsigned char size = sizeof(sc_packet_build);
	unsigned char type = SC_PACKET_BUILD;

	bool do_build;
	char building_type;
	int id;
	float x, y;
};

struct sc_packet_temperature
{
	unsigned char size = sizeof(sc_packet_temperature);
	unsigned char type = SC_PACKET_TEMPERATURE;

	unsigned char terrain_X;
	char terrain_Y[SIGHT_Y];
};

struct sc_packet_temperatureX
{
	unsigned char size = sizeof(sc_packet_temperatureX);
	unsigned char type = SC_PACKET_TEMPERATUREX;

	float terrainX;
	char terrainline_Y[SIGHT_Y];
};

struct sc_packet_temperatureY
{
	unsigned char size = sizeof(sc_packet_temperatureY);
	unsigned char type = SC_PACKET_TEMPERATUREY;

	float terrainY;
	char terrainline_X[SIGHT_X];
};

#pragma pack (pop)