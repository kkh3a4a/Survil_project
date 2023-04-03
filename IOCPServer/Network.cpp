
#include "Network.h"
#include "Player.h"
#include "Citizen.h"
#include"Resource.h"

#include<iostream>

std::array<Object*, MAXOBJECT> objects;
HANDLE h_iocp;

WSA_OVER_EX::WSA_OVER_EX()
{
	return;
}

WSA_OVER_EX::WSA_OVER_EX(IOCPOP iocpop, char byte, void* buf)
{
	ZeroMemory(&_wsaover, sizeof(_wsaover));
	_iocpop = iocpop;
	_wsabuf.buf = reinterpret_cast<char*>(buf);
	_wsabuf.len = byte;
}

void WSA_OVER_EX::processpacket(int client_id, unsigned char* pk)
{
	unsigned char packet_type = pk[1];
	Player* player = reinterpret_cast<Player*>(objects[client_id]);

	switch (packet_type)
	{
	case CS_PACKET_LOGIN:
	{
		cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(pk);
		send_login_ok_packet(client_id);
		send_citizen_First_create_packet(client_id);
		send_resource_First_create_packet(client_id);
		player->send_terrain_All();
		break;
	}

	case CS_PACKET_MOVE:
	{
		cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(pk);
		player->w = packet->w;
		player->a = packet->a;
		player->s = packet->s;
		player->d = packet->d;
		break;
	}
	case CS_PACKET_CITIZENPLACEMENT:
	{
		//추후 건물도 똑같이 작동할거니깐 건물도 추가해주면 좋음

		cs_packet_citizenplacement* packet = reinterpret_cast<cs_packet_citizenplacement*>(pk);
		if (packet->objectid >= RESOURCESTART && packet->objectid < RESOURCESTART + MAXRESOURCE) 
		{
			Resource* resource = reinterpret_cast<Resource*> (objects[packet->objectid]);
			resource->set_resource_citizen_placement(client_id, packet->isplus);
		}
		break;
	}
	default:
	{
		closesocket(reinterpret_cast<Player*>(objects[client_id])->_socket);
		DebugBreak();
		break;
	}
	}
}

void WSA_OVER_EX::send_login_ok_packet(int client_id)
{
	sc_packet_login packet;
	Player* player = reinterpret_cast<Player*>(objects[client_id]);
	
	packet.x = player->_x;
	packet.y = player->_y;
	packet.z = player->_z;
	packet.player_id = player->_id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LOGIN;

	player->send_packet(&packet);
}

void WSA_OVER_EX::send_citizen_First_create_packet(int client_id)
{
	
	Player* player = reinterpret_cast<Player*>(objects[client_id]);

	for (int i = 0; i < MAXPLAYER; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			sc_packet_citizencreate packet;
			packet.x = objects[i * 200 + j + 5]->_x;
			packet.y = objects[i * 200 + j + 5]->_y;
			packet.z = objects[i * 200 + j + 5]->_z;
			packet.citizenid = i * 200 + j + 5;

			packet.size = sizeof(sc_packet_citizencreate);
			packet.type = SC_PACKET_CITIZENCREATE;

			player->send_packet(&packet);
		}
	}
	

}

void WSA_OVER_EX::send_resource_First_create_packet(int client_id)
{
	Player* player = reinterpret_cast<Player*>(objects[client_id]);
	for (int i = RESOURCESTART; i < MAXRESOURCE + RESOURCESTART; ++i)
	{
		Resource* resource = reinterpret_cast<Resource*>(objects[i]);
		sc_packet_resourcecreate packet;

		packet.x = resource->_x;
		packet.y = resource->_y;
		packet.z = resource->_z;
		packet.resource_type = resource->_type;
		packet.resourceid = i;
		packet.amount = resource->_amount;

		packet.size = sizeof(sc_packet_resourcecreate);
		packet.type = SC_PACKET_RESOURCECREATE;

		player->send_packet(&packet);
	}
}

void WSA_OVER_EX::set_accept_over()
{
	_iocpop = IOCPOP::OP_ACCEPT;
}
