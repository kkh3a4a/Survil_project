// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "MeshTerrain.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "../../../../IOCPServer/protocol.h"
#include "Temperature.generated.h"

UCLASS()
class PROJECT_API ATemperature : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATemperature();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	ADecalActor* DecalActor;
	TArray<UMaterialInstanceDynamic*> MaterialInstanceArray;
	TArray<ADecalActor*> DecalArray;
	bool IsHidden = false;

	const int TemperatureDivide = 4;
	const int _DecalSize = 11;
	const int ColorsInDecalX = 20;
	const int ColorsInDecalY = 20;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
		UMaterial* TemperatureMaterial;

	uint8 TerrainTemperature[SIGHT_X][SIGHT_Y];
	//TArray<int32>TerrainTemperature32;
	void TemperatureToRGB(double , double& , double& , double& );
	void Hide(bool);
	bool GetIsHidden();
};
