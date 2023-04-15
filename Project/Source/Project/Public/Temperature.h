// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "MeshTerrain.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "../../../../IOCPServer/protocol.h"
#include "Temperature.generated.h"

class FTemperatureThread : public FRunnable
{
public:
	FTemperatureThread();
	void Stop();
	virtual uint32_t Run() override;

	bool LineX = false;
	bool LineY = false;

	/*char* TerrainLineX;
	char* TerrainLineY;*/
	char TerrainLineX[SIGHT_X];
	char TerrainLineY[SIGHT_Y];

	float x;
	float y;

	ATemperature* TemperatureClass;
	bool Running = true;
private:


};

UCLASS()
class PROJECT_API ATemperature : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATemperature();
	~ATemperature();

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
	void TemperatureToRGB(double , double& , double& , double& );
	void Hide(bool);
	bool GetIsHidden();

	FRunnableThread* WorkThread;
	FTemperatureThread* Work;
	bool ReadyToUpdate = false;
};
