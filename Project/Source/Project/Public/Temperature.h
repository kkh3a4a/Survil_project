// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Temperature.generated.h"

UCLASS()
class PROJECT_API ATemperature : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATemperature();

protected:
	UDecalComponent* TemperatureDecal;
	UMaterialInstance* Material1;
	UMaterialInstance* Material2;
	UMaterialInstance* Material3;
	UMaterialInstance* Material4;
	UMaterialInstance* Material5;
	UMaterialInstance* Material6;
	UMaterialInstance* Material7;

public:	
	void Initiaize(UMaterialInstance*);
	void Update();

};
