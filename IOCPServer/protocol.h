#pragma once

constexpr int one_side_number = 3200;
constexpr int random_array_size = 10000000;
constexpr int city_size = 100;

#define BUFSIZE						512
#define SERVERPORT					9000

#define MAXNAMESIZE					20
#define MAXOBJECT					5000

#define MAXPLAYER					5
#define ROOMPLAYER					1

#define CITIZENSTART				MAXPLAYER
#define FIRSTCITIZENCREATE			20

#define PLAYERCITIZENCOUNT			200
#define MAXCITIZEN					1000
#define RESOURCESTART				(MAXCITIZEN + CITIZENSTART)
#define MAXRESOURCE					50
#define BUILDINGSTART				(MAXRESOURCE + RESOURCESTART)
#define PLAYERBUILDINGCOUNT			121
#define MAXBUILDING					(PLAYERBUILDINGCOUNT * MAXPLAYER)
#define ARMYSTART					(BUILDINGSTART + MAXBUILDING)
#define PLAYERARMYCOUNT				20
#define ARMYMAX						(PLAYERARMYCOUNT * MAXPLAYER)
#define EVENTSTART					(ARMYSTART + ARMYMAX)
#define EVENTMAX					200


#define SIGHT_X 200
#define SIGHT_Y 100


#define CS_PACKET_LOGIN				1
#define CS_PACKET_MOVE				2
#define CS_PACKET_CITIZENCREATE		3
#define CS_PACKET_CITIZENMOVE		4
#define CS_PACKET_RESOURCECREATE	5
#define CS_PACKET_SUNANGLE			6
#define CS_PACKET_CITIZENREMOVE		7
#define CS_PACKET_MINIMAP			8

#define CS_PACKET_CITIZENPLACEMENT	10

#define CS_PACKET_RESOURCEAMOUNT	20
#define CS_PACKET_PLAYERRESOURCE	21

#define CS_PACKET_TERRAINALL		30
#define CS_PACKET_TERRAINXLOCATION	31
#define CS_PACKET_TERRAINYLOCATION	32

#define CS_PACKET_BUILDABLE			40
#define CS_PACKET_BUILD				41

#define CS_PACKET_TEMPERATURE		50

#define CS_PACKET_ARMYTRAINING		60
#define CS_PACKET_ARMYMOVE			61
#define CS_PACKET_ARMYRETURN		62
#define CS_PACKET_ARMYDISBAND		63

#define CS_PACKET_VIEWEVENT			70
#define CS_PACKET_REMOVEEVENT		71
#define CS_PACKET_EVENTSELECT		72

#define CS_PACKET_POLICY			80


#define SC_PACKET_LOGIN				1
#define SC_PACKET_MOVE				2
#define SC_PACKET_CITIZENCREATE		3
#define SC_PACKET_CITIZENMOVE		4
#define SC_PACKET_RESOURCECREATE	5
#define SC_PACKET_SUNANGLE			6
#define SC_PACKET_CITIZENREMOVE		7
#define SC_PACKET_MINIMAP			8

#define SC_PACKET_CITIZENPLACEMENT	10
#define SC_PACKET_RESOURCEAMOUNT	20
#define SC_PACKET_PLAYERRESOURCE	21

#define SC_PACKET_TERRAINALL		30
#define SC_PACKET_TERRAINXLOCATION	31
#define SC_PACKET_TERRAINYLOCATION	32

#define SC_PACKET_BUILDABLE			40
#define SC_PACKET_BUILD				41
#define SC_PACKET_BUILDSUCCESS		42

#define SC_PACKET_TEMPERATURE		50
#define SC_PACKET_TEMPERATUREX		51
#define SC_PACKET_TEMPERATUREY		52

#define SC_PACKET_ARMYTRAINING		60
#define SC_PACKET_ARMYMOVE			61
#define SC_PACKET_ARMYDISBAND		63

#define SC_PACKET_VIEWEVENT			70
#define SC_PACKET_REMOVEEVENT		71
#define SC_PACKET_EVENTSELECT		72

#define SC_PACKET_POLICY_ACCEPT 	80
#define SC_PACKET_POLICY_TICKET 	81


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

struct cs_packet_minimap
{
	unsigned char size;
	unsigned char type;

	float x, y;
};

struct cs_packet_armymove
{
	unsigned char size;
	unsigned char type;

	float x, y;
	int a_id;
};

struct cs_packet_eventselect
{
	unsigned char size;
	unsigned char type;

	int s_option;
	int e_id;
};

struct cs_packet_armyreturn
{
	unsigned char size;
	unsigned char type;

	int a_id;
};

struct cs_packet_armydisband
{
	unsigned char size;
	unsigned char type;

	int a_id;
};

struct cs_packet_policy
{
	unsigned char size = sizeof(cs_packet_policy);
	unsigned char type = CS_PACKET_POLICY;

	int policy_id;
	// 0: 든든한 식사, 1:스프밥, 2: 밀주
	// 3: 연명치료, 4: 극약처방, 5: 과잉수용, 6: 환자추가배식, 7: 마약치료 8: 요양원, 9: 의수족
	// 10: 장례식, 11: 시체저장소, 12: 장기이식, 13: 식인, 14: 추모비
	// 15: 자경단, 16: 경비초소, 17: 교도소, 18: 계엄령, 19: 독재자
	// 20: 비상교대근무, 21: 연장근무, 22: 감찰관, 23: 강제노동
	// 24: 아동노동, 25: 보육원, 26: 학도병징집, 27: 작업지원
};




struct sc_packet_login
{
	unsigned char size;
	unsigned char type;
	float x, y, z;
	float rx, ry, rz;
	float p1_x, p1_y, p1_z;
	float p2_x, p2_y, p2_z;
	float p3_x, p3_y, p3_z;
	float p4_x, p4_y, p4_z;
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
	char citizenstate;
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

struct sc_packet_buildsuccess
{
	unsigned char size = sizeof(sc_packet_buildsuccess);
	unsigned char type = SC_PACKET_BUILDSUCCESS;

	int id;
	char building_type;
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

struct  sc_packet_armytraining
{
	unsigned char size = sizeof(sc_packet_armytraining);
	unsigned char type = SC_PACKET_ARMYTRAINING;

	int c_id1, c_id2, c_id3, c_id4, c_id5;
	float x, y, z;
	int army_id;
};

struct sc_packet_armydisband
{
	unsigned char size;
	unsigned char type;

	int issuccess;
	int a_id;
};

struct sc_packet_armymove
{
	unsigned char size;
	unsigned char type;
	int a_id;
	float x, y, z;
	float rx, ry, rz;
	char  a_state;
};

struct sc_packet_viewevnet
{
	unsigned char size;
	unsigned char type;

	int e_id;
	float x, y, z;
};

struct sc_packet_removeevent
{
	unsigned char size;
	unsigned char type;

	int e_id;
};

struct sc_packet_eventselect
{
	unsigned char size;
	unsigned char type;

	int s_option;
	wchar_t summary[30]{};
	wchar_t first[20]{}, second[20]{}, third[20]{};
	int e_id;
	int e_type;
};

struct sc_packet_policy_ticket
{
	unsigned char size = sizeof(sc_packet_policy_ticket);
	unsigned char type = SC_PACKET_POLICY_TICKET;

	int ticket;
};

struct sc_packet_policy_accept
{
	unsigned char size = sizeof(sc_packet_policy_accept);
	unsigned char type = SC_PACKET_POLICY_ACCEPT;

	bool accept;
	int ticket;
};

#pragma pack (pop)