
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

void WSA_OVER_EX::processpacket(int client_id, unsigned char* pk)
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

		break;
	}

	default:
	{
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
}
