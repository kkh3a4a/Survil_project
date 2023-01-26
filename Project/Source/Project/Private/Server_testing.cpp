// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Server_testing.h"
#include "Kismet/GameplayStatics.h"
#include "citizen.h"
// Sets default values

//AMyPlayerController* my_controller;

AServer_testing::AServer_testing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

int8 AServer_testing::get_height(int32 x, int32 y)
{
	return terrain_array[x][y];
}

// Called when the game starts or when spawned
void AServer_testing::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> CitizensToFind;
	wcout.imbue(locale("korean"));
	ret = WSAStartup(MAKEWORD(2, 2), &WSAData);
	s_socket = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	ret = inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

	//connect();
	ret = connect(s_socket, reinterpret_cast<sockaddr*> (&server_addr), sizeof(server_addr));
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), Acitizen::StaticClass(), FName("Citizen"), CitizensToFind);

		for (AActor* MY_CIT : CitizensToFind)

		{

			Acitizen* CitizenCast = Cast<Acitizen>(MY_CIT);

			if (CitizenCast)
			{
				My_Citizen.Add(*MY_CIT->GetName(), MY_CIT);
				//UE_LOG(LogTemp, Log, TEXT("%s %lf %lf"), *MY_CIT->GetName(), MY_CIT->GetActorLocation().X, MY_CIT->GetActorLocation().Y);
			}

		}
	}
	int tmp = My_Citizen.Num();
	ret = send(s_socket, (char*)&tmp, sizeof(int), 0);
	//UE_LOG(LogTemp, Log, TEXT("%d"), tmp);
	for (auto& a : My_Citizen)
	{
		wcscpy(test_Actor.name, *a.Key);
		test_Actor.location.x = a.Value->GetActorLocation().X;
		test_Actor.location.y = a.Value->GetActorLocation().Y;
		test_Actor.location.z = a.Value->GetActorLocation().Z;
		UE_LOG(LogTemp, Log, TEXT("%lf %lf %lf"), test_Actor.location.x, test_Actor.location.y, test_Actor.location.z);
		//UE_LOG(LogTemp, Log, TEXT("%s %lf %lf"), *a.Key, a.Value->GetActorLocation().X, a.Value->GetActorLocation().Y);


		ret = send(s_socket, (char*)&test_Actor, (int)sizeof(test_Actor), 0);
	}

	UE_LOG(LogTemp, Log, TEXT("connected to server"));

	start_t = high_resolution_clock::now();
}

// Called every frame
void AServer_testing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	steady_clock::time_point end_t = high_resolution_clock::now();
	//UE_LOG(LogTemp, Log, TEXT("%s : %lf, %lf"), *MouseInput.name, MouseInput.location.x, MouseInput.location.y);
	//UE_LOG(LogTemp, Log, TEXT("%d"), test);
	//my_controller->testFunction();
	//UE_LOG(LogTemp, Log, TEXT("%d"), test);
	if (duration_cast<milliseconds>(end_t - start_t).count() > 50) {

		start_t = high_resolution_clock::now();
		ret = send(s_socket, (char*)&MouseInput, (int)sizeof(FActor_location_rotation), 0);
		if (SOCKET_ERROR == ret)
		{
			return;
		}
		ret = recv(s_socket, (char*)&sunangle, (int)sizeof(Fthree_float), 0);
		if (SOCKET_ERROR == ret)
		{
			return;
		}
		int cnt = 0;
		for (auto& a : My_Citizen) {
			cnt++;
			ret = recv(s_socket, (char*)&test_Actor, (int)sizeof(FActor_location_rotation), 0);
			if (SOCKET_ERROR == ret)
			{
				return;
			}
			FVector tmp = { test_Actor.location.x, test_Actor.location.y, test_Actor.location.z };
			a.Value->SetActorLocation(tmp);
		}
		//UE_LOG(LogTemp, Log, TEXT("%d %lf %lf"), cnt, MYplayer_controller->MouseInput.location.x, MYplayer_controller->MouseInput.location.y)
		//UE_LOG(LogTemp, Log, TEXT("%d %lf %lf"), cnt, MouseInput.location.x, MouseInput.location.y)
		
		//===================
		FOneArray temp_array;
		terrain_array.Init(temp_array, map_size);
		
		for (int i = 0; i < map_size; i++)
		{
			//terrain_array[i].Empty();
			ret = recv(s_socket, (char*)&terrain_2d_array, (int)(sizeof(char) * map_size), 0);
			if (SOCKET_ERROR == ret)
			{
				return;
			}
			for (int j = 0; j < map_size; j++)
			{
				terrain_array[i].Add(terrain_2d_array[j]);
				//UE_LOG(LogTemp, Log, TEXT("%d %d %d"), i, j, terrain_array[i][j]);
			}
		}
		UE_LOG(LogTemp, Log, TEXT("map load once"));
		//=====================
	}


	//UE_LOG(LogTemp, Log, TEXT("%f, %f, %f"), test_Actor.location.x , test_Actor.location.y, test_Actor.location.z);
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

