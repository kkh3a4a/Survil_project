// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Windows/AllowWindowsPlatformTypes.h"
// include header with included Windows.h
#include<iostream>
#include <chrono>
#include "global.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "GameFramework/Actor.h"
#include "Citizen.h"
#include "MyTown.h"
#include "MeshTerrain.h"
#include "Temperature.h"
#include "Building.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Main.generated.h"

using namespace std;
using namespace chrono;

UCLASS()
class PROJECT_API AMain : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AMain();
	~AMain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool RecvedUIInput = true;
	bool IsConnected = false;
	bool RecvedFirstData = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUI_Input UI_Input;

	//////////////UI때문에 여기있어야함
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
		int oil_count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
		int water_count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
		int iron_count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
		int food_count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
		int wood_count = 0;

	TMap<int, FActorTransform*> players_list;

	bool ThreadInitSendRecv = false;

	bool CitizenRelaese = false;
	
	FKeyInput* KeyInput = new FKeyInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Citizen_num = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CitizenNoJobCnt = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Citizen")
		TSubclassOf<AActor>CitizenActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Citizen")
		TSubclassOf<AActor>EnemyCitizenActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TSubclassOf<AActor>WellPump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TSubclassOf<AActor>OilActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TSubclassOf<AActor>WaterActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TSubclassOf<AActor>IronActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TSubclassOf<AActor>FoodActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TSubclassOf<AActor>WoodActor;

	//citizen
	ACitizen* Citizens;
	AMyTown* MyTown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SunAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
		UMaterialInstance* TerrainMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature")
		UMaterial* TemperatureMaterial;

	int8 Terrain2DArray[MapSizeX][MapSizeY];
	uint8 TerrainTemperature[MapSizeX][MapSizeY];

	AMeshTerrain* TerrainActor;
	ATemperature* Temperature;

	ACameraActor* MyCamera;

	FThreeFloat OldLocation;
	FThreeFloat FutureLocation;
	FVector InterpolatedLocation;
	FVector OldInterpolatedLocation;
	double CycleTime{};
	int CycleNum{};

	ABuilding* Building;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
		UMaterial* BuildingGridMaterial;


	void TF_set(FThreeFloat& a, FThreeFloat& b);
	void resoure_set(Fresources_actor& a, Fresources_actor& b);
	void LocationInterpolate();


	//////////////////////////////////////////
	class FSocketThread* Network;
	float Player_x, Player_y, Player_z;
	void SetPlayerLocation(float x, float y, float z);
	void SetCurrentLocation(float x, float y, float z);

};

