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

	UI_Input.ResourceInput.ResourceNum = -1;
	UI_Input.ResourceInput.CitizenCountAdd = false;
	UI_Input.ResourceInput.CitizenCountSub = false;
	
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
	
	memcpy(&ClientStruct1.KeyInput, KeyInput, sizeof(FKeyInput));
	KeyInput = &ClientStruct1.KeyInput;
	Network = new FSocketThread(this);
	NetworkThread = FRunnableThread::Create(Network, TEXT("MyThread"), 0, TPri_BelowNormal);
}

// Called every frame
void AServer_testing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	steady_clock::time_point StartTime = high_resolution_clock::now();
	steady_clock::time_point EndTime = StartTime;
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
		
		memcpy(&ClientStruct1.UIInput, &UI_Input.ResourceInput, sizeof(FUI_Input));

		clock_t t_1 = clock();
		oil_count = ServerStruct1.MyResource[0], water_count = ServerStruct1.MyResource[1], iron_count = ServerStruct1.MyResource[2], food_count = ServerStruct1.MyResource[3], wood_count = ServerStruct1.MyResource[4];
		SunAngle = ServerStruct1.SunAngle;
		Citizens->CitizenNoJob(CitizenNoJobCnt);
		Citizens->Citizen_Moving();
		TerrainActor->UpdateMeshTerrain(Terrain2DArray);
		Temperature->Update(TerrainTemperature);
		MyTown->UpdateResource();

		LocationInterpolate();
		//SetActorLocation(FVector(ServerStruct1.CurrentLocation.x - MapSizeX * 100 / 2, ServerStruct1.CurrentLocation.y - MapSizeY * 100 / 2, ServerStruct1.CurrentLocation.z));
	}
	EndTime = high_resolution_clock::now();
	CycleTime = duration_cast<milliseconds>(EndTime - StartTime).count();
}

void AServer_testing::TF_set(FThreeFloat& a, FThreeFloat& b)
{
	a.x = b.x;
	a.y = b.y;
	a.z = b.z;
}

void AServer_testing::resoure_set(Fresources_actor& a, Fresources_actor& b)
{
	a.Count = b.Count;
	a.Type = b.Type;
}

void AServer_testing::LocationInterpolate()
{
	double Alpha;

	//updateµÆÀ»¶§
	if (CycleNum == 0) {
		CycleNum++;
		OldLocation = FutureLocation;
		FutureLocation = ServerStruct1.CurrentLocation;
		OldInterpolatedLocation = InterpolatedLocation;
		InterpolatedLocation = FVector(OldLocation.x, OldLocation.y, OldLocation.z);
	}
	else {
		CycleNum++;
		Alpha = CycleTime * (CycleNum) / Network->CycleTime;
		OldInterpolatedLocation = InterpolatedLocation;
		InterpolatedLocation = FMath::Lerp(FVector(OldLocation.x, OldLocation.y, 0), FVector(FutureLocation.x, FutureLocation.y, 0), min(Alpha, (double)0.9f));
	}
	
	/*if (abs(InterpolatedLocation.X - OldInterpolatedLocation.X) > 100) {
		UE_LOG(LogTemp, Warning, TEXT("CycleTime: %lf, NetCycle: %lf, CycleNum: %d, Alpha: %lf"), CycleTime, Network->CycleTime, CycleNum, Alpha);
		UE_LOG(LogTemp, Warning, TEXT("Curr  : %f, %f"), ServerStruct1.CurrentLocation.x, ServerStruct1.CurrentLocation.y);
		UE_LOG(LogTemp, Warning, TEXT("Old   : %f, %f"), OldLocation.x, OldLocation.y);
		UE_LOG(LogTemp, Warning, TEXT("Future: %f, %f"), FutureLocation.x, FutureLocation.y);
		UE_LOG(LogTemp, Warning, TEXT("InterDiff :[%lf, %lf]"), abs(InterpolatedLocation.X - OldInterpolatedLocation.X), abs(InterpolatedLocation.Y - OldInterpolatedLocation.Y));
		UE_LOG(LogTemp, Warning, TEXT("Inter :[%lf, %lf]"), InterpolatedLocation.X, InterpolatedLocation.Y);
	}*/
	//UE_LOG(LogTemp, Warning, TEXT("InterDiff :[%lf, %lf]"), abs(InterpolatedLocation.X - OldInterpolatedLocation.X), abs(InterpolatedLocation.Y - OldInterpolatedLocation.Y));

	SetActorLocation(FVector((int)InterpolatedLocation.X - MapSizeX * 100 / 2, (int)InterpolatedLocation.Y - MapSizeY * 100 / 2, 0));
}

