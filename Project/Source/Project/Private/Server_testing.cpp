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

FSocketThread* Network;
FRunnableThread* NetworkThread;

AServer_testing::AServer_testing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AServer_testing::~AServer_testing()
{
	if (NetworkThread != nullptr)
	{
		Network->Stop();
	}
}

// Called when the game starts or when spawned
void AServer_testing::BeginPlay()
{
	Super::BeginPlay();
	
	KeyInput->w = false;
	KeyInput->a = false;
	KeyInput->s = false;
	KeyInput->d = false;
	//Citizen
	FActorSpawnParameters SpawnInfo;
	Citizens = GetWorld()->SpawnActor<ACitizen>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	Citizens->Initialize(CitizenActor, EnemyCitizenActor);

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
	
	memcpy(&ClientStruct1.My_keyboard_input, KeyInput, sizeof(Fkeyboard_input));
	KeyInput = &ClientStruct1.My_keyboard_input;
	Network = new FSocketThread(this);
	NetworkThread = FRunnableThread::Create(Network, TEXT("MyThread"), 0, TPri_BelowNormal);
}

// Called every frame
void AServer_testing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!RecvedFirstData){
		if (ThreadInitSendRecv){
			Citizens->citizen_set(ServerStruct1, ServerStruct2);
			Citizens->Spawn_Citizen();
			MyTown->SpawnTown(players_list);
			MyTown->SpawnResource(ServerStruct1, ServerStruct2);
			Citizens->Citizen_Moving();
			RecvedFirstData = true;
		}
	}
	else {
		if (RecvedUIInput == false){
			if (UI_Input.resouce_input.CitizenCountAdd || UI_Input.resouce_input.CitizenCountSub){
				RecvedUIInput = true;
			}
		}
		else if (RecvedUIInput == true){
			if (!UI_Input.resouce_input.CitizenCountAdd && !UI_Input.resouce_input.CitizenCountSub){
				RecvedUIInput = false;
			}
			UI_Input.resouce_input.CitizenCountAdd = false;
			UI_Input.resouce_input.CitizenCountSub = false;
		}
		memcpy(&ClientStruct1.My_UI_input, &UI_Input.resouce_input, sizeof(FUI_Input));

		clock_t t_1 = clock();
		oil_count = ServerStruct1.MyResource[0], water_count = ServerStruct1.MyResource[1], iron_count = ServerStruct1.MyResource[2], food_count = ServerStruct1.MyResource[3], wood_count = ServerStruct1.MyResource[4];
		TF_set(CurrentLocation, ServerStruct1.currlocation);
		SunAngle = ServerStruct1.SunAngle;

		Citizens->CitizenNoJob(CitizenNoJobCnt);
		Citizens->Citizen_Moving();

		TerrainActor->UpdateMeshTerrain(Terrain2DArray);
		Temperature->Update(TerrainTemperature);

		SetActorLocation(FVector(CurrentLocation.x - MapSizeX * 100 / 2, CurrentLocation.y - MapSizeY * 100 / 2, CurrentLocation.z));
		MyTown->UpdateResource();
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
