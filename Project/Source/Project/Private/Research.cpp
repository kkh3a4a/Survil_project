// Fill out your copyright notice in the Description page of Project Settings.


#include "Research.h"
#include "NetworkingThread.h"


// Sets default values
AResearch::AResearch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AResearch::BeginPlay()
{
	Super::BeginPlay();
	
}
void AResearch::set_NetWork(void* NetWork)
{
	_NetWork = reinterpret_cast<FSocketThread*>(NetWork);
}
void AResearch::Send_Technology(int type, int level)
{
    cs_packet_technology packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_TECHNOLOGY;
	packet.tech_level = level;
	packet.tech_type = type;

    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(_NetWork->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}
void AResearch::Send_Techphase(int phase)
{
	cs_packet_techphase packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_TECHPHASE;
	packet.tech_phase = phase;

	WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
	WSASend(_NetWork->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}
void AResearch::set_technology(int type, int level)
{
	switch (type)
	{
	case 0:
	{
		tech1 = level;
		break;
	}
	case 1:
	{
		tech2 = level;
		break;
	}
	case 2:
	{
		tech3 = level;
		break;
	}
	case 3:
	{
		tech4 = level;
		break;
	}
	case 4:
	{
		tech5 = level;
		break;
	}
	default:
		break;
	}

}

void AResearch::set_tech_phase(int phase)
{
	tech_phase = phase;
}









//
// Called every frame
