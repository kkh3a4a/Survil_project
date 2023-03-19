// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DecalActor.h"
#include "Building.generated.h"

UCLASS()
class PROJECT_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

protected:
	ADecalActor* DecalActor;

public:	
	bool BuildMode = false;
	FVector DecalLocation;
	
	void Initialize(UMaterial*);
	void Update();
	void DecalVisibility();
};
