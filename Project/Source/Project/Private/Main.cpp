// Fill out your copyright notice in the Description page of Project Settings.
#include "Main.h"
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


FRunnableThread* NetworkThread;

AMain::AMain()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AMain::~AMain()
{
	if (NetworkThread != nullptr)
	{
		if(Network != nullptr)
			Network->Stop();
	}
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
	KeyInput->w = false;
	KeyInput->a = false;
	KeyInput->s = false;
	KeyInput->d = false;
	//Citizen
	FActorSpawnParameters SpawnInfo;
	/*Citizens = GetWorld()->SpawnActor<ACitizen>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	Citizens->Initialize(CitizenActor, EnemyCitizenActor);*/

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

	//Spawn Building
	Building = GetWorld()->SpawnActor<ABuilding>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	Building->Initialize(BuildingGridMaterial);

	
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
	Network = new FSocketThread();
	Network->_MainClass = this;
	NetworkThread = FRunnableThread::Create(Network, TEXT("MyThread"), 0, TPri_BelowNormal);
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//steady_clock::time_point StartTime = high_resolution_clock::now();
	//steady_clock::time_point EndTime = StartTime;
	//if (!RecvedFirstData){
	//	if (ThreadInitSendRecv){
	//		Citizens->citizen_set(ServerStruct1, ServerStruct2);
	//		Citizens->Spawn_Citizen();
	//		MyTown->SpawnTown(players_list);
	//		MyTown->SpawnResource(ServerStruct1, ServerStruct2);
	//		Citizens->Citizen_Moving();
	//		RecvedFirstData = true;
	//	}
	//}
	//else {
	//	
	//	memcpy(&ClientStruct1.UIInput, &UI_Input.ResourceInput, sizeof(FUI_Input));

	//	clock_t t_1 = clock();
	//	oil_count = ServerStruct1.MyResource[0], water_count = ServerStruct1.MyResource[1], iron_count = ServerStruct1.MyResource[2], food_count = ServerStruct1.MyResource[3], wood_count = ServerStruct1.MyResource[4];
	//	SunAngle = ServerStruct1.SunAngle;
	//	Citizens->CitizenNoJob(CitizenNoJobCnt);
	//	Citizens->Citizen_Moving();
	//	TerrainActor->UpdateMeshTerrain(Terrain2DArray);
	//	Temperature->Update(TerrainTemperature);
	//	Building->Update();
	//	MyTown->UpdateResource();

	//	LocationInterpolate();
	//	//SetActorLocation(FVector(ServerStruct1.CurrentLocation.x - MapSizeX * 100 / 2, ServerStruct1.CurrentLocation.y - MapSizeY * 100 / 2, ServerStruct1.CurrentLocation.z));
	//}
	//EndTime = high_resolution_clock::now();
	//CycleTime = duration_cast<milliseconds>(EndTime - StartTime).count();
}


void AMain::SetPlayerLocation(float x, float y, float z)
{
	Player_x = x; 
	Player_y = y;
	Player_z = z;
	FVector Location(Player_x, Player_y, Player_z);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	GetWorld()->SpawnActor<AActor>(WellPump, Location, Rotation, SpawnInfo);

	SetActorLocation(FVector(x, y, z));
}

void AMain::SetCurrentLocation(float current_x, float current_y, float current_z)
{
	SetActorLocation(FVector(Player_x + current_x, Player_y + current_y, Player_z + current_z));
}

