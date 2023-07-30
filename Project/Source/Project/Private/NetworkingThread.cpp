// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkingThread.h"
#include "Main.h"
#include "Citizen.h"
#include "MyPlayerController.h"
#include "CitizenManager.h"
#include "ResourceManager.h"
#include "Components/AudioComponent.h"
using namespace std;

FSocketThread* fsocket_thread;
//UE_LOG(LogTemp, Warning, TEXT("%d"), (int)packet->citizenstate);
NetworkingThread::NetworkingThread()
{
	
}

NetworkingThread::~NetworkingThread()
{
	
}

WSA_OVER_EX::WSA_OVER_EX()
{
	//exit(-1);
	return;
}

WSA_OVER_EX::WSA_OVER_EX(IOCPOP iocpop, char byte, void* buf)
{
	ZeroMemory(&_wsaover, sizeof(_wsaover));
	_iocpop = iocpop;
	_wsabuf.buf = reinterpret_cast<char*>(buf);
	_wsabuf.len = byte;
}

FSocketThread::FSocketThread()
{
	
}

uint32_t FSocketThread::Run()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Network Construct Start")));

	fsocket_thread = this;
	WSADATA WSAData;
	int ret = WSAStartup(MAKEWORD(2, 0), &WSAData);
	if (ret != 0)
	{
		//exit(-1);
	}
	s_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, IPAddress, &server_addr.sin_addr);
	ret = WSAConnect(s_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr), 0, 0, 0, 0);
	if (ret != 0)
	{
		//	exit(-1);
	}
	UE_LOG(LogTemp, Warning, TEXT("Network Thread connect!!"));

	cs_packet_login packet_login;
	packet_login.size = sizeof(packet_login);
	packet_login.type = CS_PACKET_LOGIN;
	WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(IOCPOP::OP_SEND, sizeof(cs_packet_login), &packet_login);
	ret = WSASend(s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
	if (ret != 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Send Error in Network Construct")));
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Successfully Send in Network Construct")));


	DWORD r_flags = 0;
	ZeroMemory(&_recv_over_ex, sizeof(_recv_over_ex));
	_recv_over_ex._wsabuf.buf = reinterpret_cast<char*>(_recv_over_ex._buf);
	_recv_over_ex._wsabuf.len = sizeof(_recv_over_ex._buf);
	ret = WSARecv(s_socket, &_recv_over_ex._wsabuf, 1, 0, &r_flags, &_recv_over_ex._wsaover, recv_callback);
	if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Recv Error in Network Construct")));
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Successfully Received in Network Construct")));
	IsRunning = true;
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("finished construct network")));
	while (_MainClass == nullptr || _MyController == nullptr || _CitizenManager == nullptr || _ResourceManager == nullptr || _BuildManager == nullptr)
	{
		Sleep(10);
	}

	int iter = 0;
	while (IsRunning)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Network Running %d"), iter++));
		SleepEx(100, true);
	}
	UE_LOG(LogTemp, Warning, TEXT("Network Thread End!!"));
	return 0;
}

void FSocketThread::error_display(const char* msg, int err_no)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	LocalFree(lpMsgBuf);
}

void FSocketThread::processpacket(unsigned char* buf)
{
	unsigned char packet_type = buf[1];
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Packet Type: %d"), (int)packet_type));
	if(IsRunning)
	{
		switch (packet_type)
		{
		case SC_PACKET_LOGIN:
		{
			sc_packet_login* packet = reinterpret_cast<sc_packet_login*>(buf);
			//=====================================================================================================================================================
			//수정완료
			/*_MainClass->SetPlayerLocation(packet->x, packet->y, packet->z);
			_MainClass->SetOtherPlayerLocation(packet->p1_x, packet->p1_y, packet->p1_z);
			_MainClass->SetOtherPlayerLocation(packet->p2_x, packet->p2_y, packet->p2_z);
			_MainClass->SetOtherPlayerLocation(packet->p3_x, packet->p3_y, packet->p3_z);
			_MainClass->SetOtherPlayerLocation(packet->p4_x, packet->p4_y, packet->p4_z);*/

			_MainClass->PlayersLocation_X[0] = packet->x;
			_MainClass->PlayersLocation_Y[0] = packet->y;
			
			_MainClass->PlayersLocation_X[1] = packet->p1_x;
			_MainClass->PlayersLocation_Y[1] = packet->p1_y;
			
			_MainClass->PlayersLocation_X[2] = packet->p2_x;
			_MainClass->PlayersLocation_Y[2] = packet->p2_y;
			
			_MainClass->PlayersLocation_X[3] = packet->p3_x;
			_MainClass->PlayersLocation_Y[3] = packet->p3_y;
			
			_MainClass->PlayersLocation_X[4] = packet->p4_x;
			_MainClass->PlayersLocation_Y[4] = packet->p4_y;

			_MainClass->PlayersLocationReady = true;
			
			_MainClass->DestLocation = FVector(packet->x, packet->y, packet->z) + FVector(-SIGHT_X * 100 / 2, -SIGHT_Y * 100 / 2, packet->z);
			_MyController->maxplayercount = packet->maxplayer;
			_MyController->connectplayercount = packet->connectplayer;
			_MyController->my_id = my_id = packet->player_id;
			break;
		}
		case SC_PACKET_MOVE:
		{
			sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(buf);
			_MainClass->DestLocation = FVector(packet->currentX, packet->currentY, packet->currentZ) + FVector(_MainClass->Player_x, _MainClass->Player_y, _MainClass->Player_z);
			break;
		}
		case SC_PACKET_CITIZENCREATE:
		{
			sc_packet_citizencreate* packet = reinterpret_cast<sc_packet_citizencreate*>(buf);
			//=====================================================================================================================================================
			// //수정완료
			//_CitizenManager->Spawn_Citizen(packet->citizenid - CITIZENSTART, FVector(packet->x, packet->y, packet->z));
			//_CitizenManager->PutCitizenForSpawn(packet->citizenid - CITIZENSTART, FVector(packet->x, packet->y, packet->z));
			_CitizenManager->SetCitizenQueue(packet);
			break;
		}
		case SC_PACKET_CITIZENMOVE:
		{
			sc_packet_citizenmove* packet = reinterpret_cast<sc_packet_citizenmove*>(buf);
			//=====================================================================================================================================================
			//수정 완
			FRotator Rotation = (FVector(packet->rx, packet->ry, packet->rz)).GetSafeNormal().Rotation();
			_CitizenManager->Set_Citizen_Move_Queue(packet);
			break;
		}
		case SC_PACKET_RESOURCECREATE:
		{
			sc_packet_resourcecreate* packet = reinterpret_cast<sc_packet_resourcecreate*>(buf);
			//=====================================================================================================================================================
			//수정 완료
			_ResourceManager->Set_Resource_Queue(packet->resource_id - RESOURCESTART, FVector(packet->x, packet->y, packet->z), packet->amount, packet->resource_type);
			break;
		}
		case SC_PACKET_RESOURCEAMOUNT:
		{
			sc_packet_resourceamount* packet = reinterpret_cast<sc_packet_resourceamount*>(buf);
			_ResourceManager->SetResourceAmount(packet->resourceid - RESOURCESTART, packet->amount);
			break;
		}
		case SC_PACKET_CITIZENREMOVE:
		{
			sc_packet_citizenremove* packet = reinterpret_cast<sc_packet_citizenremove*>(buf);
			_CitizenManager->Set_Citizen_Remove_Queue(packet);
			break;
		}
		case SC_PACKET_PLAYERRESOURCE:
		{
			sc_packet_playerresource* packet = reinterpret_cast<sc_packet_playerresource*>(buf);
			_MainClass->SetPlayerResource(packet->resources_acount[0], packet->resources_acount[1], packet->resources_acount[2], packet->resources_acount[3], packet->resources_acount[4]);
			break;
		}
		case SC_PACKET_CITIZENPLACEMENT:
		{
			sc_packet_citizenplacement* packet = reinterpret_cast<sc_packet_citizenplacement*>(buf);
			playerjobless = packet->playerjobless;
			if (packet->object_id >= RESOURCESTART && packet->object_id < RESOURCESTART + MAXRESOURCE)
			{
				_ResourceManager->SetResourcePlacement(packet->object_id - RESOURCESTART, packet->workcitizen);
			}
			else if (packet->object_id >= BUILDINGSTART && packet->object_id < BUILDINGSTART + MAXBUILDING)
			{
				_BuildManager->SetBuildingPlacement(packet->object_id - BUILDINGSTART, packet->workcitizen);
			}
			break;
		}
		case SC_PACKET_TERRAINALL:
		{
			sc_packet_terrainAll* packet = reinterpret_cast<sc_packet_terrainAll*>(buf);
			memcpy(&_MainClass->Terrain->Terrain2DArray[packet->terrain_X], packet->terrain_Y, SIGHT_Y);
			_MainClass->Terrain->ReadyToUpdate = true;
			break;
		}
		case SC_PACKET_TERRAINXLOCATION:
		{
			sc_packet_terrainXlocation* packet = reinterpret_cast<sc_packet_terrainXlocation*>(buf);
			_MainClass->Terrain->Work->LineX = true;
			_MainClass->Terrain->Work->x = packet->terrainX;
			memcpy(&_MainClass->Terrain->Work->TerrainLineY, packet->terrainline_Y, SIGHT_Y);
			break;
		}
		case SC_PACKET_TERRAINYLOCATION: 
		{
			sc_packet_terrainYlocation* packet = reinterpret_cast<sc_packet_terrainYlocation*>(buf);
			_MainClass->Terrain->Work->LineY = true;
			_MainClass->Terrain->Work->y = packet->terrainY;
			memcpy(&_MainClass->Terrain->Work->TerrainLineX, packet->terrainline_X, SIGHT_X);
			break;
		}
		case SC_PACKET_SUNANGLE:
		{
			sc_packet_sunangle* packet = reinterpret_cast<sc_packet_sunangle*>(buf);
			//=====================================================================================================================================================
			//수정완료
			_MainClass->SunAngle = packet->sunangle;
			break;
		}
		case SC_PACKET_BUILDABLE:
		{
			sc_packet_buildable* packet = reinterpret_cast<sc_packet_buildable*>(buf);
			_MainClass->BuildManager->Buildable = packet->buildable;
			break;
		}
		case SC_PACKET_BUILD:
		{
			// 수정 완
			sc_packet_build* packet = reinterpret_cast<sc_packet_build*>(buf);
			if (packet->do_build) {
				_MainClass->BuildManager->SetBuildQueue(packet);
			}
			break;
		}
		case SC_PACKET_BUILDSUCCESS:
		{
			// 수정 완
			sc_packet_buildsuccess* packet = reinterpret_cast<sc_packet_buildsuccess*>(buf);
			_MainClass->BuildManager->SetBuildSuccessQueue(packet);
			if (packet->building_type == 6) //연구소일시 연구소 수량 증가, 연구 버튼 활성화
			{
				_MainClass->Research->count_lab++;
			}
			break;
		}
		case SC_PACKET_TEMPERATURE:
		{
			sc_packet_temperature* packet = reinterpret_cast<sc_packet_temperature*>(buf);
			memcpy(&_MainClass->Temperature->TerrainTemperature[packet->terrain_X], packet->terrain_Y, SIGHT_Y);
			_MainClass->Temperature->ReadyToUpdate = true;
			break;
		}
		case SC_PACKET_TEMPERATUREX:
		{
			sc_packet_temperatureX* packet = reinterpret_cast<sc_packet_temperatureX*>(buf);
			_MainClass->Temperature->Work->LineX = true;
			_MainClass->Temperature->Work->x = packet->terrainX;
			memcpy(&_MainClass->Temperature->Work->TerrainLineY, packet->terrainline_Y, SIGHT_Y);
			break;
		}
		case SC_PACKET_TEMPERATUREY:
		{
			sc_packet_temperatureY* packet = reinterpret_cast<sc_packet_temperatureY*>(buf);
			_MainClass->Temperature->Work->LineY = true;
			_MainClass->Temperature->Work->y = packet->terrainY;
			memcpy(&_MainClass->Temperature->Work->TerrainLineX, packet->terrainline_X, SIGHT_X);
			break;
		}
		case SC_PACKET_ARMYTRAINING:
		{
			sc_packet_armytraining* packet = reinterpret_cast<sc_packet_armytraining*>(buf);
			//=====================================================================================================================================================
			//수정완료
			_CitizenManager->Set_Army_Queue(packet);
			break;
		}
		case SC_PACKET_ARMYMOVE:
		{
			sc_packet_armymove* packet = reinterpret_cast<sc_packet_armymove*>(buf);
			FRotator Rotation = (FVector(packet->rx, packet->ry, packet->rz)).GetSafeNormal().Rotation();
			//=====================================================================================================================================================
			//수정완료
			_CitizenManager->Set_Army_Move_Queue(packet);
			break;
		}
		case SC_PACKET_ARMYCHANGEHP:
		{
			sc_packet_armychangehp* packet = reinterpret_cast<sc_packet_armychangehp*>(buf);
			_CitizenManager->Set_Army_Hp(packet->hp, packet->army_id - ARMYSTART);
			break;
		}
		case SC_PACKET_ARMYDEAD:
		{
			sc_packet_armydead* packet = reinterpret_cast<sc_packet_armydead*>(buf);
			_CitizenManager->Army_Dead(packet->army_id - ARMYSTART);
			break;
		}
		case SC_PACKET_ARMYATTACK:
		{
			sc_packet_armyattack* packet = reinterpret_cast<sc_packet_armyattack*>(buf);
			FRotator Rotation = (FVector(packet->rx, packet->ry, packet->rz)).GetSafeNormal().Rotation();
			//_CitizenManager->Set_Army_Attack(packet->army_id - ARMYSTART, Rotation, packet->a_state);
			_CitizenManager->Set_Army_Attack_Queue(packet);
			break;
		}
		case SC_PACKET_VIEWEVENT:
		{
			sc_packet_viewevnet* packet = reinterpret_cast<sc_packet_viewevnet*>(buf);
			_ResourceManager->Set_Event_Queue(packet->e_id - EVENTSTART, FVector(packet->x, packet->y, packet->z));
			break;
		}
		case SC_PACKET_REMOVEEVENT:
		{
			sc_packet_removeevent* packet = reinterpret_cast<sc_packet_removeevent*>(buf);
			_ResourceManager->Set_Remove_Event_Queue(packet->e_id - EVENTSTART);
			break;
		}
		case SC_PACKET_EVENTSELECT:
		{
			sc_packet_eventselect* packet = reinterpret_cast<sc_packet_eventselect*>(buf);
			_MyController->select_event(packet);
			break;
		}
		case SC_PACKET_ARMYDISBAND:
		{
			sc_packet_armydisband* packet = reinterpret_cast<sc_packet_armydisband*>(buf);
			if (packet->issuccess)
				_CitizenManager->Set_Army_Disband(packet->a_id - ARMYSTART);
			break;
		}
		case SC_PACKET_TRADEREQUEST:
		{
			sc_packet_traderequest* packet = reinterpret_cast<sc_packet_traderequest*>(buf);
			if (_MyController->TradeUI)
			{
				_MyController->Trade_DisAccess_send(2, packet->request_player);
				break;
			}
			_MyController->Trade_Request_UI(packet->request_player);
			break;
		}
		case SC_PACKET_TRADEAGREE:
		{
			sc_packet_tradeagree* packet = reinterpret_cast<sc_packet_tradeagree*>(buf);
			if (packet->isagree == 0 || packet->isagree == 2)
			{
				_MyController->Trade_UI_visible = false;
			}
			break;
		}
		case SC_PACKET_TRADERESOURCE:
		{
			sc_packet_traderesource* packet = reinterpret_cast<sc_packet_traderesource*>(buf);
			_MyController->trade_change_resource(packet->resource_num, packet->resource_amount);
			break;
		}
		case SC_PACKET_TRADEDEAL:
		{
			sc_packet_tradedeal* packet = reinterpret_cast<sc_packet_tradedeal*>(buf);
			_MyController->other_trade_deal = (bool)packet->deal_boolean;
			break;
		}
		case SC_PACKET_TRADESUCCESS:
		{
			sc_packet_tradesuccess* packet = reinterpret_cast<sc_packet_tradesuccess*>(buf);
			_MyController->other_trade_success = (bool)packet->success_boolean;

			break;
		}
		case SC_PACKET_TECHNOLOGY:
		{
			sc_packet_technology* packet = reinterpret_cast<sc_packet_technology*>(buf);
			_MainClass->Research->set_technology(packet->tech_type, packet->tech_level);
			UE_LOG(LogTemp, Warning, TEXT("technology : %d, %d"), packet->tech_type, packet->tech_level);
			break;
		}
		case SC_PACKET_TECHPHASE:
		{
			sc_packet_techphase* packet = reinterpret_cast<sc_packet_techphase*>(buf);
			_MainClass->Research->set_tech_phase(packet->tech_phase);
			break;
		}
		case SC_PACKET_TECHTIMER:
		{
			sc_packet_techtimer* packet = reinterpret_cast<sc_packet_techtimer*>(buf);
			_MainClass->Research->tech_timer = packet->tech_timer_second;
			_MainClass->Research->set_tech_timer();
			break;
		}
		case SC_PACKET_DECLARATION_WAR:
		{
			sc_packet_declaration_war* packet = reinterpret_cast<sc_packet_declaration_war*>(buf);
			_MyController->set_war_player(packet->player_num, packet->is_war);
			break;
		}
		case SC_PACKET_SPRINKLER_OFF:	//스프링클러 다 끄기
		{
			sc_packet_sprinkler_off* packet = reinterpret_cast<sc_packet_sprinkler_off*>(buf);
			for (int i = 0; i < MAXBUILDING; i++) {
				if (_MainClass->BuildManager->BuiltBuildings[i] != nullptr) {
					if (FCString::Atoi(*_MainClass->BuildManager->BuiltBuildings[i]->Tags[1].ToString()) == 8) {
						if (_MainClass->BuildManager->BuiltBuildings[i]->Tags[4] == TEXT("ON")) {
							_MainClass->BuildManager->BuiltBuildings[i]->Tags.Remove(FName("ON"));
							_MainClass->BuildManager->BuiltBuildings[i]->Tags.Add(FName("OFF"));
						}
					}
				}
			}
			break;
		}
		case SC_PACKET_SANDSTORMDAY:
		{
			sc_packet_sandstormday* packet = reinterpret_cast<sc_packet_sandstormday*>(buf);
			_MyController->sand_storm_day = packet->sand_day;
			if (_MyController->sand_storm_day == 0)
			{
				_ResourceManager->eventreset = true;
			}
			//=====================================================================================================================================================
			//수정완료
			/*if (_MyController->sand_storm_day == 0)
			{
				Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Music/event/SandStorm_Cue.SandStorm_Cue"));
				SoundComponent = UGameplayStatics::SpawnSound2D(_MyController->GetWorld(), Sound);
				UE_LOG(LogTemp, Log, TEXT("PlaySoundSandStorm\n"));
				
				_MyController->PlaySandStormAnim = true;
				
			}
			else if (_MyController->sand_storm_day != 0 && SoundComponent && SoundComponent->IsPlaying())
			{
				SoundComponent->Stop();
				_MyController->PlaySandStormAnim = false;
			}*/
			break;
		}
		case SC_PACKET_CITIZEN_STATUS:
		{
			sc_packet_citizen_status* packet = reinterpret_cast<sc_packet_citizen_status*>(buf);
			if (packet->soldier_num < 0 || packet->citizen_num < 0 || packet->citizen_hot < 0 || packet->citizen_hungry < 0 || packet->citizen_thirsty < 0)
				break;
			//UE_LOG(LogTemp, Warning, TEXT("[Citizen Status] Soldier: %d, Citizen: %d, Hot: %d, Hungry: %d, Thirsty: %d"), packet->soldier_num, packet->citizen_num, packet->citizen_hot, packet->citizen_hungry, packet->citizen_thirsty);
			_MyController->SoldierNum = packet->soldier_num;
			_MyController->CitizenNum = packet->citizen_num;
			_MyController->CitizenHotNum = packet->citizen_hot;
			_MyController->CitizenHungryNum = packet->citizen_hungry;
			_MyController->CitizenThirstyNum = packet->citizen_thirsty;
			break;
		}
		case SC_PACKET_GAMEOVER:
		{
			//=====================================================================================================================================================
			/*if (_MyController->IsGameOver == false)
			{
				Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Music/bgm/Lose_bgm_Cue.Lose_bgm_Cue"));
				UGameplayStatics::PlaySound2D(_MyController->GetWorld(), Sound);
			}*/
			if(!_MyController->Lose)
				_MyController->Lose = true;
			break;
		}
		case SC_PACKET_GAMEEND:
		{
			//=====================================================================================================================================================
			/*if (_MyController->GameEnd == false)
			{
				Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Music/bgm/Win_bgm_Cue.Win_bgm_Cue"));
				UGameplayStatics::PlaySound2D(_MyController->GetWorld(), Sound);
			}*/
			if(!_MyController->Win)
				_MyController->Win = true;
			_MyController->set_ending(buf);
			break;
		}
		case SC_PACKET_WIND_DIRECTION:
		{
			sc_packet_wind_direction* packet = reinterpret_cast<sc_packet_wind_direction*>(buf);
			//=====================================================================================================================================================
			//수정완료
			_MainClass->Terrain->WindDirectionChanged = true;
			_MainClass->Terrain->WindDirection_X = (int)packet->wind_x;
			_MainClass->Terrain->WindDirection_Y = (int)packet->wind_y;
			UE_LOG(LogTemp, Log, TEXT("Wind Dir: X: %d, Y: %d"), (int)packet->wind_x, (int)packet->wind_y);
			break;
		}
		case SC_PACKET_DISSATISFACTION:
		{
			sc_packet_dissatisfaction* packet = reinterpret_cast<sc_packet_dissatisfaction*>(buf);
			_MainClass->Dissatisfaction = packet->dissatisfaction;
			break;
		}
		case SC_PACKET_MATCHING:
		{
			sc_packet_matching* packet = reinterpret_cast<sc_packet_matching*>(buf);
			_MyController->maxplayercount = packet->maxplayer;
			_MyController->connectplayercount = packet->connectplayer;
			break;
		}
		default:
		{
			//DebugBreak();
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("UNKNOWN Packet Type: %d"), (int)packet_type));
			break;
		}
		}
	}
}

void FSocketThread::Stop()
{
	IsRunning = false;
}

void CALLBACK send_callback(DWORD err, DWORD num_byte, LPWSAOVERLAPPED send_over, DWORD flag)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("send callback start")));
	if (err != 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("send callback ERROR")));
		return;
	}
	WSA_OVER_EX* wsa_over_ex = reinterpret_cast<WSA_OVER_EX*>(send_over);
	delete  wsa_over_ex;
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("send callback done")));
}

void CALLBACK recv_callback(DWORD err, DWORD num_byte, LPWSAOVERLAPPED recv_over, DWORD flag) 
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("recv callback start")));
	if (err != 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("recv callback ERROR")));
		return;
	}
	WSA_OVER_EX* wsa_over_ex = reinterpret_cast<WSA_OVER_EX*>(recv_over);
	
	//여기서 패킷 재조립을 해준다.
	unsigned char* packet_start = wsa_over_ex->_buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static unsigned char packet_buffer[BUFSIZE];

	while (0 != num_byte) {
		if (0 == in_packet_size) in_packet_size = packet_start[0];
		if (num_byte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, packet_start, in_packet_size - saved_packet_size);
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("recv callback num bytes: %d"), in_packet_size));
			fsocket_thread->processpacket(packet_buffer);
			packet_start += in_packet_size - saved_packet_size;
			num_byte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, packet_start, num_byte);
			saved_packet_size += num_byte;
			num_byte = 0;
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Packet 잘림")));
		}
	}

	ZeroMemory(&wsa_over_ex->_wsaover, sizeof(wsa_over_ex->_wsaover));
	DWORD r_flags = 0;
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("recv callback done")));
	WSARecv(fsocket_thread->s_socket, &fsocket_thread->_recv_over_ex._wsabuf, 1, 0, &r_flags, &fsocket_thread->_recv_over_ex._wsaover, recv_callback);
}