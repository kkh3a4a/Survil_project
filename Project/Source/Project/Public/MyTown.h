// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include"global.h"
#include "GameFramework/Actor.h"
#include "MyTown.generated.h"

UCLASS()
class PROJECT_API AMyTown : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTown();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	
		
	TMap<int, Fresources_actor> resources_create_landscape;
	TArray<AActor*> ResourceEditer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>WellPump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>OilActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>WaterActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>IronActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>FoodActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>WoodActor;

	int resources[5] = {};

	void Initialize(TSubclassOf<AActor>, TSubclassOf<AActor>, TSubclassOf<AActor>, TSubclassOf<AActor>, TSubclassOf<AActor>, TSubclassOf<AActor>);
	void SpawnTown(TMap<int, FActor_location_rotation>&);
	void SpawnResource();
};
