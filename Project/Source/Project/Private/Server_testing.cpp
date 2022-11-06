// Fill out your copyright notice in the Description page of Project Settings.


#include "Server_testing.h"

// Sets default values
AServer_testing::AServer_testing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AServer_testing::BeginPlay()
{
	Super::BeginPlay();

	wcout.imbue(locale("korean"));
	ret = WSAStartup(MAKEWORD(2, 2), &WSAData);
	s_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	ret = inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

	//connect();
	ret = connect(s_socket, reinterpret_cast<sockaddr*> (&server_addr), sizeof(server_addr));

	/*hThread = CreateThread(NULL, 0, AServer_testing::Angle_Receiver,
		(LPVOID)s_socket, 0, NULL);*/
	
}

// Called every frame
void AServer_testing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (SOCKET_ERROR == ret)
	{
		return;
	}
	ret = recv(s_socket, (char*)&sunangle, (int)sizeof(SunAngle), 0);
	UE_LOG(LogTemp, Log, TEXT("%f, %f, %f"), sunangle.x , sunangle.y, sunangle.z);
	//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow, TEXT(" %d, %d, %d", sunangle.x, sunangle.y, sunangle.z));

}
//
//DWORD WINAPI AServer_testing::Angle_Receiver(LPVOID arg)
//{
//	while (1) {
//		if (SOCKET_ERROR == ret)
//		{
//			break;
//		}
//		ret = send(s_socket, (char*)&sunangle, (int)sizeof(SunAngle), 0);
//	}
//}

