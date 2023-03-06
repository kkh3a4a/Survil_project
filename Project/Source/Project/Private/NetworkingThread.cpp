// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkingThread.h"
#include "Server_testing.h"
#include "Kismet/GameplayStatics.h"

using namespace chrono;
using namespace std;
AServer_testing* ServerClass_send_recv;
NetworkingThread::NetworkingThread()
{
	
}

NetworkingThread::~NetworkingThread()
{
	
}

FSocketThread::FSocketThread(AActor* temp_server_testing)
{
	ServerClass_send_recv = Cast<AServer_testing>(temp_server_testing);

	
}

void FSocketThread::ServerGetter()
{

}

//bool FSocketThread::Init()
//{
//
//}

uint32_t FSocketThread::Run()
{
	/*while (!(ServerClass_send_recv->maxplayer_cnt == MAXPLAYER))
	{
		recv(ServerClass_send_recv->s_socket, (char*)&ServerClass_send_recv->maxplayer_cnt, sizeof(int), 0);
		send(ServerClass_send_recv->s_socket, (char*)&ServerClass_send_recv->trash_value, sizeof(int), 0);
	}

	recv(ServerClass_send_recv->s_socket, (char*)&ServerClass_send_recv->maxplayer_cnt, sizeof(int), 0);
	send(ServerClass_send_recv->s_socket, (char*)&ServerClass_send_recv->trash_value, sizeof(int), 0);*/

	recv(ServerClass_send_recv->s_socket, (char*)&ServerClass_send_recv->FirstSendServer, sizeof(ServerClass_send_recv->FirstSendServer), 0);

	for (int thread_cnt_num = 0; thread_cnt_num < MAXPLAYER; ++thread_cnt_num) {
		ServerClass_send_recv->players_list.Add(thread_cnt_num, &(ServerClass_send_recv->FirstSendServer.player_info));
	}
	ServerClass_send_recv->first_recv_send = true;

	UE_LOG(LogTemp, Log, TEXT("connected to server"));

	ServerClass_send_recv->Citizens->Citizen_moving->team = -1;
	ServerClass_send_recv->Citizens->Citizen_moving->citizen_number = -1;


	memcpy(&ServerClass_send_recv->FirstSendClient.My_citizen_moving, ServerClass_send_recv->Citizens->Citizen_moving, sizeof(FCitizen_moving));

	ServerClass_send_recv->Citizens->Citizen_moving = &ServerClass_send_recv->FirstSendClient.My_citizen_moving;


	ServerClass_send_recv->Isthreading_first_send = true;

	steady_clock::time_point start_t = high_resolution_clock::now();
	while(1)
	{
		steady_clock::time_point end_t = high_resolution_clock::now();
		if (duration_cast<milliseconds>(end_t - start_t).count() > 50)
		{
			start_t = high_resolution_clock::now();
			ServerClass_send_recv->temped += 1;
			recv(ServerClass_send_recv->s_socket, (char*)&ServerClass_send_recv->FirstSendServer, sizeof(ServerClass_send_recv->FirstSendServer), 0);

			for (int thread_cnt_num = 0; thread_cnt_num < MapSizeX; thread_cnt_num++) {
				ServerClass_send_recv->ret = recv(ServerClass_send_recv->s_socket, (char*)&ServerClass_send_recv->Terrain2DArray[thread_cnt_num], sizeof(char) * MapSizeY, 0);
				if (SOCKET_ERROR == ServerClass_send_recv->ret) {
					return 0;
				}
			}
			//Recv Temperature
			for (int thread_cnt_num = 0; thread_cnt_num < MapSizeX; thread_cnt_num++) {
				ServerClass_send_recv->ret = recv(ServerClass_send_recv->s_socket, (char*)&ServerClass_send_recv->TerrainTemperature[thread_cnt_num], sizeof(char) * MapSizeY, 0);
				if (SOCKET_ERROR == ServerClass_send_recv->ret) {
					return 0;
				}
			}


			send(ServerClass_send_recv->s_socket, (char*)&ServerClass_send_recv->FirstSendClient, sizeof(FFirstSendClient), 0);
		}
	}
	return 0;
}

//void FSocketThread::Stop()
//{
//    
//}
