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
#include "Camera/CameraComponent.h"
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
	MyCameraComponent = MyCamera->GetCameraComponent();
	HeatHazeMaterialInstance = UMaterialInstanceDynamic::Create(HeatHazeMaterial, this);
	MyCameraComponent->PostProcessSettings.WeightedBlendables.Array[0].Object = HeatHazeMaterialInstance;

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
	Research = reinterpret_cast<AResearch*>(Research);
	Research->set_NetWork(Network);
	applied_policy.SetNum(28);
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Research->tech_timer >= 0)
	{
		Research->tech_timer -= DeltaTime;
	}
	MyCameraComponent->PostProcessSettings.VignetteIntensity = abs(0.5 * cos((SunAngle - 90)* PI / 180.0) - 0.5);
	if(SunAngle < 180)
		HeatHazeMaterialInstance->SetScalarParameterValue(FName("Strength"), abs(cos((SunAngle + 90) * PI / 180.0)) / 100);


	FVector CurrentLocation = GetActorLocation();
	float DistanceToDest = FVector::Dist(CurrentLocation, DestLocation);
	float MoveSpeed = DeltaTime * 120 * DistanceToDest / 7;
	if (DistanceToDest <= 1.0f){
		return;
	}
	FVector NewLocation = FMath::Lerp(CurrentLocation, DestLocation,  MoveSpeed / DistanceToDest);
	SetActorLocation(NewLocation);
	Temperature->SetActorLocation(DestLocation);
	Terrain->SetActorLocation(DestLocation);

	//UE_LOG(LogTemp, Warning, TEXT("%lf, %lf"), DeltaTime, MoveSpeed);
	//UE_LOG(LogTemp, Warning, TEXT("%lf, %lf, %lf    %lf, %lf, %lf    %lf, %lf, %lf"), CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z, NewLocation.X, NewLocation.Y, NewLocation.Z, DestLocation.X, DestLocation.Y, DestLocation.Z);

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
	//Terrain->SetActorLocation(FVector(Player_x, Player_y, 0.0));
	//Temperature->SetActorLocation(FVector(Player_x - SIGHT_X * 100 / 2, Player_y - SIGHT_Y * 100 / 2, 0.0));
	//Terrain->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//Temperature->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SetActorLocation(FVector(x, y, z));
}

void AMain::SetOtherPlayerLocation(float x, float y, float z)
{
	
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	UWorld* uworld = GetWorld();
	FVector Location(x, y, z);
	if (uworld != nullptr)
		uworld->SpawnActor<AActor>(WellPump, Location, Rotation, SpawnInfo);
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

bool AMain::SendPolicyPacket(int PolicyID)
{
	if (policy_ticket > 0) {
		if (applied_policy[PolicyID]) return false;

		switch (PolicyID) {
		case 2: if (!applied_policy[0] && !applied_policy[1]) return false; break;
		case 5: if (!applied_policy[3] && !applied_policy[4]) return false; break;
		case 6: if (!applied_policy[3] && !applied_policy[4]) return false; break;
		case 7: if (!applied_policy[5] && !applied_policy[6]) return false; break;
		case 8: if (!applied_policy[3] && !applied_policy[4]) return false; break;
		case 9: if (!applied_policy[8]) return false; break;
		case 12: if (!applied_policy[10] && !applied_policy[11]) return false; break;
		case 13: if (!applied_policy[12]) return false; break;
		case 14: if (!applied_policy[10] && !applied_policy[11]) return false; break;
		case 16: if (!applied_policy[15]) return false; break;
		case 17: if (!applied_policy[16]) return false; break;
		case 18: if (!applied_policy[16]) return false; break;
		case 19: if (!applied_policy[18]) return false; break;
		case 21: if (!applied_policy[20]) return false; break;
		case 22: if (!applied_policy[20]) return false; break;
		case 23: if (!applied_policy[22]) return false; break;
		case 26: if (!applied_policy[24] && !applied_policy[25]) return false; break;
		case 27: if (!applied_policy[24] && !applied_policy[25]) return false; break;
		}

		cs_packet_policy packet;
		packet.policy_id = PolicyID;
		UE_LOG(LogTemp, Log, TEXT("send Policy"));
		WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
		WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
		return true;
	}
	else {
		//∫Œ¡∑
		UE_LOG(LogTemp, Log, TEXT("no policy ticket"));
		return false;
	}
}
