// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Windows/AllowWindowsPlatformTypes.h"
// include header with included Windows.h
#include<iostream>
#include <chrono>
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
#include "BuildManager.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
//#include "UMGEditor/Public/WidgetBlueprint.h"
#include "Research.h"
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

	bool ThreadInitSendRecv = false;
	bool CitizenRelaese = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TSubclassOf<AActor>WellPump;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SunAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
		UMaterial* HeatHazeMaterial;
	UMaterialInstanceDynamic* HeatHazeMaterialInstance;


	AMeshTerrain* Terrain;
	ATemperature* Temperature;

	ACameraActor* MyCamera;
	UCameraComponent* MyCameraComponent;
	AActor* SunManager;
	ABuildManager* BuildManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AResearch* Research;
	//////////////////////////////////////////
	class FSocketThread* Network;
	float Player_x, Player_y, Player_z;
	void SetPlayerLocation(float x, float y, float z);
	void SetOtherPlayerLocation(float x, float y, float z);
	void SetPlayerResource(int oilcount, int watercount, int ironcount, int foodcount, int woodcount);

	void SetSunAngle();
	int testterrain = 0;

	FVector DestLocation;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray <bool> applied_policy;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Dissatisfaction = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentDate = 1;
	float PrevSunAngle = 0.f;


	bool PlayersLocationReady = false;
	float PlayersLocation_X[5];
	float PlayersLocation_Y[5];

};
