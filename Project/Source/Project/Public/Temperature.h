// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "MeshTerrain.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/DecalComponent.h"
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
	const int TemperatureDivide = 4;
	TArray<UMaterialInstanceDynamic*> MaterialInstanceArray;
	TArray<ADecalActor*> DecalArray;
	bool IsHidden = false;
	float _DecalSize = 22;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature")
		UMaterial* TemperatureMaterial;

	uint8 TerrainTemperature[MapSizeX][MapSizeY];
	void TemperatureToRGB(double , double& , double& , double& );
	void Hide(bool);
	bool GetIsHidden();
};
