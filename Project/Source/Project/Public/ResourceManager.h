// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceManager.generated.h"

UCLASS()
class PROJECT_API AResourceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class AMain* Main;

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

	AActor* resources[50];
	int resource_amount[50]{};	//resource.cpp을 만들기보다 여기서 같이 관리하자
	char resource_type[50]{};	//type도 마찬가지
	char workCitizens[50]{};		// 몇명 일하는것도 마찬가지
	char playerjobless = {10};
	class FSocketThread* Network;
	void Spawn_Resource(int Resource_id, FVector Location, int amount, char resourcetype);
	void SetResourceAmount(int Resource_id, int amount);
	void SetResourcePlacement(int Resource_id, char work_citizen, char player_job_less);
};
