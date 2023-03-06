// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "MeshTerrain.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Temperature.generated.h"

UCLASS()
class PROJECT_API ATemperature : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATemperature();
	

protected:

	int divide = 4;
	ADecalActor* DecalActor;
	TArray<UMaterialInstanceDynamic*> MaterialInstanceArray;
	TArray<ADecalActor*> DecalArray;
	bool IsHidden = false;

public:	
	void Initiaize(UMaterial*);
	void Update(int8(*TerrainTemperaturePtr)[MapSizeY]);
	void TemperatureToRGB(double , double& , double& , double& );

	void Hide(bool);
	bool GetIsHidden();
};
