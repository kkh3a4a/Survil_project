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
	
	FActorSpawnParameters SpawnInfo;
	
	//Get BuildManager
	AActor* BuildManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABuildManager::StaticClass());
	if (BuildManagerActor == nullptr) {
		return;
	}
	BuildManager = Cast <ABuildManager>(BuildManagerActor);
	if (BuildManager == nullptr) {
		return;
	}

	//Get Temperature
	AActor* TemperatureActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATemperature::StaticClass());
	if (TemperatureActor == nullptr) {
		return;
	}
	Temperature = Cast <ATemperature>(TemperatureActor);
	if (Temperature == nullptr) {
		return;
	}
	//TemperatureActor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Get Terrain
	AActor* TerrainActor = UGameplayStatics::GetActorOfClass(GetWorld(), AMeshTerrain::StaticClass());
	if (TerrainActor == nullptr) {
		return;
	}
	Terrain = Cast <AMeshTerrain>(TerrainActor);
	if (Terrain == nullptr) {
		return;
	}

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

	//Get Sun
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
	for (AActor* Actor : Actors) {
		//UE_LOG(LogTemp, Warning, TEXT("Actor name: %s"), *Actor->GetName());
		if (Actor->GetName() == "SunManager_C_1") {
			SunManager = Actor;
			UE_LOG(LogTemp, Warning, TEXT("FOUND SUN MANAGER"));
			break;
		}
	}

	//Newtwork
	Network = new FSocketThread();
	Network->_MainClass = this;
	Network->_BuildManager = BuildManager;
	NetworkThread = FRunnableThread::Create(Network, TEXT("MyThread"), 0, TPri_BelowNormal);
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AMain::SetPlayerLocation(float x, float y, float z)
{
	Player_x = x; 
	Player_y = y;
	Player_z = z;
	FVector Location(Player_x, Player_y, Player_z);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	UWorld* uworld = GetWorld();
	if(uworld != nullptr)
	uworld->SpawnActor<AActor>(WellPump, Location, Rotation, SpawnInfo);
	Terrain->SetActorLocation(FVector(Player_x, Player_y, 0.0));
	Temperature->SetActorLocation(FVector(Player_x - SIGHT_X * 100 / 2, Player_y - SIGHT_Y * 100 / 2, 0.0));
	//Terrain->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//Temperature->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SetActorLocation(FVector(x, y, z));
}

void AMain::SetCurrentLocation(float current_x, float current_y, float current_z)
{
	SetActorLocation(FVector(Player_x + current_x, Player_y + current_y, Player_z + current_z));
}

void AMain::SetPlayerResource(int oilcount, int watercount, int ironcount, int foodcount, int woodcount)
{
	oil_count = oilcount;
	water_count = watercount;
	iron_count = ironcount;
	food_count = foodcount;
	wood_count = woodcount;
}

void AMain::SetSunAngle(float s_sunangle)
{
	SunAngle = s_sunangle;
	if (SunManager != nullptr)
	{
		SunManager->SetActorRotation(FRotator(SunAngle, 0.f, 0.f));
	}
}
