// Fill out your copyright notice in the Description page of Project Settings.
#include "Main.h"
#include <math.h>
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include <future>
#include <thread>
#include <mutex>
#include "MyGameInstance.h"
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

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetLevel()));
	
	//Newtwork
	Network = new FSocketThread();
	if (GameInstance)
	{
		const TCHAR* TCHARString = *GameInstance->input_IPAddress;

		// TCHAR 문자열 길이 계산
		int32 TCHARLength = FCString::Strlen(TCHARString);

		// 필요한 버퍼 크기 계산
		int32 BufferSize = WideCharToMultiByte(CP_UTF8, 0, TCHARString, TCHARLength, nullptr, 0, nullptr, nullptr);

		// BufferSize가 OutCharArray 크기보다 큰 경우, 적절한 크기의 버퍼를 할당하지 않고 함수를 종료합니다.
		ZeroMemory(&Network->IPAddress, 20);
		// TCHAR 배열을 멀티바이트 문자열로 변환
		WideCharToMultiByte(CP_UTF8, 0, TCHARString, TCHARLength, Network->IPAddress, BufferSize, nullptr, nullptr);

	}
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

	if (PlayersLocationReady) {
		SetPlayerLocation(PlayersLocation_X[0], PlayersLocation_Y[0], 0);
		for (int i = 1; i < 5; i++)
			SetOtherPlayerLocation(PlayersLocation_X[i], PlayersLocation_Y[i], 0);
		PlayersLocationReady = false;
	}

	SetSunAngle();


	if (Research->TechTime > 0)
	{
		FDateTime CurrentTime = FDateTime::UtcNow();
		int64 CurrentTimeUnixTimestamp = CurrentTime.ToUnixTimestamp();
		int PastTime = CurrentTimeUnixTimestamp - Research->TechTime;
		Research->LeftTechTime = Research->tech_timer - PastTime;
		//UE_LOG(LogTemp, Warning, TEXT("Research->TechTime: %lld, CurrentTimeUnixTimestamp: %lld, PastTime: %d, LeftTechTime: %d"), Research->TechTime, CurrentTimeUnixTimestamp, PastTime, Research->LeftTechTime);

		if (PastTime >= Research->tech_timer) {
			Research->TechTime = 0;
			Research->LeftTechTime = 0;
			Research->tech_timer = 0;
		}
	}

	if (SunAngle < PrevSunAngle)
		CurrentDate++;
	PrevSunAngle = SunAngle;
	
	MyCameraComponent->PostProcessSettings.VignetteIntensity = abs(0.5 * cos((SunAngle - 90)* PI / 180.0) - 0.5);

	if(SunAngle < 180)
		HeatHazeMaterialInstance->SetScalarParameterValue(FName("Strength"), abs(cos((SunAngle + 90) * PI / 180.0)) / 100);
	else
		HeatHazeMaterialInstance->SetScalarParameterValue(FName("Strength"), 0);
	if(Temperature->GetIsHidden())
		HeatHazeMaterialInstance->SetScalarParameterValue(FName("Strength"), 0);

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
	FRotator Rotation(0.f, 0.f, 0.f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Name = FName("Well Pump");
	UWorld* uworld = GetWorld();
	if (IsValid(uworld) && IsValid(WellPump)) {
		uworld->SpawnActor<AActor>(WellPump, Location, Rotation, SpawnInfo);
	}
	SetActorLocation(FVector(x, y, z));
}

void AMain::SetOtherPlayerLocation(float x, float y, float z)
{
	
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo{};
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

void AMain::SetSunAngle()
{
	if (SunManager != nullptr)
	{
		SunManager->SetActorRotation(FRotator(SunAngle, 0.f, 0.f));
	}
}
