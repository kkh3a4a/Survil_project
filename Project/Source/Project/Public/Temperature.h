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

public:	
	void Initiaize(UMaterial*);
	void Update(int8(*TerrainTemperaturePtr)[MapSizeY]);
	void TemperatureToRGB(double , double& , double& , double& );

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ADecalActor* DecalActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ADecalActor*> DecalArray;

	TArray<UMaterialInstanceDynamic*> MaterialInstanceArray;
};
