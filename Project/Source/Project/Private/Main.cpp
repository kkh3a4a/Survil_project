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
	
	//Init Mesh Terrain
	TerrainActor = GetWorld()->SpawnActor<AMeshTerrain>(FVector(0, 0, 0), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	TerrainActor->InitializeMeshTerrain(TerrainMaterialInstance);

	
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
	//지워야함 지형 대신 받은거임///////
	if (testterrain % 1000 == 0)
	{
		for (auto& x : Terrain2DArray)
		{
			for (auto& y : x)
			{
				y = rand() % 10;
			}
		}
	}
	testterrain++;
	/////////////////////////////////////////////////
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
	TerrainActor->SetActorLocation(FVector(Player_x, Player_y, 0.0));
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

void AMain::SetTerrainActorLocation(float x, float y)
{
	static float t_x = x, t_y = y;
	if(TerrainActor != nullptr)
	{
		TerrainActor->SetActorLocation(FVector(Player_x + x, Player_y + y, 0.0));
		
	}
	if ((int)t_x != (int)x)
	{
		if ((int)t_x < (int)x)
		{
			for (int i = 0; i < SIGHT_X - 1; ++i)
			{
				for(int j=0; j < SIGHT_Y;++j)
					Terrain2DArray[i][j] = Terrain2DArray[i + 1][j];
			}
			for (int i = 0; i < SIGHT_Y; ++i)
			{
				Terrain2DArray[SIGHT_X - 1][i] = { 0 };
			}
		}
		if ((int)t_x > (int)x)
		{

			for (int i = SIGHT_X - 1; i > 0; --i)
			{
				for (int j = 0; j < SIGHT_Y; ++j)
				{
					Terrain2DArray[i][j] = Terrain2DArray[i - 1][j];
				}
			}
			for (int i = 0; i < SIGHT_Y; ++i)
			{
				Terrain2DArray[0][i] = { 0 };
			}
		}
		t_x = x;
	}

	if ((int)t_y != (int)y)
	{
		if ((int)t_y < (int)y)
		{
			for (int i = 0; i < SIGHT_X - 1; ++i)
			{
				for (int j = 0; j < SIGHT_Y - 1; ++j)
					Terrain2DArray[i][j] = Terrain2DArray[i][j + 1];
			}
			for (int i = 0; i < SIGHT_X; ++i)
			{
				//추후 받아온 terrian 넣어주면됨
				Terrain2DArray[i][SIGHT_Y - 1] = { 0 };
			}
		}
		if ((int)t_y > (int)y)
		{
			for (int i = 0; i < SIGHT_X - 1; ++i)
			{
				for (int j = SIGHT_Y - 1; j > 0 ; --j)
				{
					Terrain2DArray[i][j] = Terrain2DArray[i][j - 1];
				}
			}
			for (int i = 0; i < SIGHT_X; ++i)
			{
				//추후 받아온 terrian 넣어주면됨
				Terrain2DArray[i][0] = { 0 };
			}
		}
		t_y = y;
	}
	TerrainActor->UpdateMeshTerrain(Terrain2DArray);
}

