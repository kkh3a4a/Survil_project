// Fill out your copyright notice in the Description page of Project Settings.
#include "Server_testing.h"
#include <math.h>
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include <future>
#include <thread>
#include <mutex>
#include "NetworkingThread.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Components/InstancedStaticMeshComponent.h"

FSocketThread* MyRunnable;
FRunnableThread* MyThread;

AServer_testing::AServer_testing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AServer_testing::~AServer_testing()
{
}

// Called when the game starts or when spawned
void AServer_testing::BeginPlay()
{
	Super::BeginPlay();
	
	my_key_input->w = false;
	my_key_input->a = false;
	my_key_input->s = false;
	my_key_input->d = false;
	//Citizen
	FActorSpawnParameters SpawnInfo;
	Citizens = GetWorld()->SpawnActor<ACitizen>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	Citizens->Initialize(Citizen_Actor, EnemyCitizenActor);

	//resource
	MyTown = GetWorld()->SpawnActor<AMyTown>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	MyTown->Initialize(WellPump, OilActor, WaterActor, IronActor, FoodActor, WoodActor);

	//Init Mesh Terrain
	TerrainActor = GetWorld()->SpawnActor<AMeshTerrain>(FVector(0, 0, 0), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	TerrainActor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TerrainActor->InitializeMeshTerrain(TerrainMaterialInstance);

	//Spawn Decal
	Temperature = GetWorld()->SpawnActor<ATemperature>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	Temperature->Initiaize(TemperatureMaterial);
	Temperature->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	UI_Input.resouce_input.ResourceNum = -1;
	UI_Input.resouce_input.CitizenCountAdd = false;
	UI_Input.resouce_input.CitizenCountSub = false;
	
	//Get Camera
	TArray<AActor*> CameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), CameraActors);
	for (AActor* Camera : CameraActors) {
		UE_LOG(LogTemp, Warning, TEXT("Actor name: %s"), *Camera->GetName());
		if (Camera->GetName() == "CameraActor_1") {
			MyCamera = Cast<ACameraActor>(Camera);
			UE_LOG(LogTemp, Warning, TEXT("CameraActor_1"));
		}
	}
	
	memcpy(&ClientSendStruct.My_keyboard_input, my_key_input, sizeof(Fkeyboard_input));
	my_key_input = &ClientSendStruct.My_keyboard_input;
	MyRunnable = new FSocketThread(this);
	MyThread = FRunnableThread::Create(MyRunnable, TEXT("MyThread"), 0, TPri_BelowNormal);
}

// Called every frame
void AServer_testing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsFirstSend){
		if (Isthreading_first_send){
			Citizens->citizen_set(ServerSendStruct);
			Citizens->Spawn_Citizen();
			MyTown->SpawnTown(players_list);
			MyTown->SpawnResource(ServerSendStruct);
			Citizens->Citizen_Moving();
			IsFirstSend = true;
		}
	}
	else {
		if (Is_send_UI_input == false){
			if (UI_Input.resouce_input.CitizenCountAdd || UI_Input.resouce_input.CitizenCountSub){
				Is_send_UI_input = true;
			}
		}
		else if (Is_send_UI_input == true){
			if (!UI_Input.resouce_input.CitizenCountAdd && !UI_Input.resouce_input.CitizenCountSub){
				Is_send_UI_input = false;
			}
			UI_Input.resouce_input.CitizenCountAdd = false;
			UI_Input.resouce_input.CitizenCountSub = false;
		}
		memcpy(&ClientSendStruct.My_UI_input, &UI_Input.resouce_input, sizeof(FUI_Input));

		clock_t t_1 = clock();
		oil_count = ServerSendStruct.MyResource[0], water_count = ServerSendStruct.MyResource[1], iron_count = ServerSendStruct.MyResource[2], food_count = ServerSendStruct.MyResource[3], wood_count = ServerSendStruct.MyResource[4];
		TF_set(CurrentLocation, ServerSendStruct.currlocation);
		TF_set(sunangle, ServerSendStruct.SunAngle);
		clock_t t_2 = clock();

		Citizens->CitizenNoJob(CitizenNoJobCnt);
		Citizens->Citizen_Moving();
		clock_t t_3 = clock();

		TerrainActor->UpdateMeshTerrain(Terrain2DArray);
		clock_t t_4 = clock();

		Temperature->Update(TerrainTemperature);
		clock_t t_5 = clock();

		SetActorLocation(FVector(CurrentLocation.x - MapSizeX * 100 / 2, CurrentLocation.y - MapSizeY * 100 / 2, CurrentLocation.z));
		clock_t t_6 = clock();
		MyTown->UpdateResource();
		//UE_LOG(LogTemp, Log, TEXT("%lf, %lf, %lf, %lf, %lf"), (double)(t_6-t_5) / CLOCKS_PER_SEC, (double)(t_5 - t_4) / CLOCKS_PER_SEC, (double)(t_4 - t_3) / CLOCKS_PER_SEC, (double)(t_3 - t_2) / CLOCKS_PER_SEC, (double)(t_2 - t_1) / CLOCKS_PER_SEC);

	}
}

void AServer_testing::TF_set(Fthree_float& a, Fthree_float& b)
{
	a.x = b.x;
	a.y = b.y;
	a.z = b.z;
}

void AServer_testing::resoure_set(Fresources_actor& a, Fresources_actor& b)
{
	a.count = b.count;
	a.type = b.type;
}
