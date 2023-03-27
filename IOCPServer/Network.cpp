
#include "Network.h"
#include "Player.h"

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

void WSA_OVER_EX::processpacket(int client_id, char* pk)
{
	unsigned char packet_type = pk[1];
	Object* object = objects[client_id];

	switch (packet_type)
	{
	case CS_PACKET_LOGIN:
	{
		cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(pk);
		Player* player = reinterpret_cast<Player*>(object);
		strcpy(player->_name, packet->name);
		player->_name[MAXNAMESIZE] = NULL;
		send_login_ok_packet(client_id);
		break;
	}

	case CS_PACKET_MOVE:
	{

		break;
	}
	default:
	{
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

	packet.currentX = player->_currentX;
	packet.currentY = player->_currentY;
	packet.currentZ = player->_currentZ;
	
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LOGIN;

	player->send_packet(&packet);
}

void WSA_OVER_EX::set_accept_over()
{
	_iocpop = IOCPOP::OP_ACCEPT;
}
