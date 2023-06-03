// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Citizen.h"
#include "Army.h"
#include "CitizenManager.generated.h"

UCLASS()
class PROJECT_API ACitizenManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACitizenManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>MyCitizen_MODEL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>EnemyCitizen_MODEL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>ARMY_MODEL;

	UPROPERTY(EditAnywhere)
	class AMain* Main;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AActor* citizen[1000];
	AActor* army[100];
	class FSocketThread* Network;
	void Spawn_Citizen(int citizen_id, FVector Location);
	void Set_Citizen_Location(int citizen_id, FVector Location, FRotator Rotate, char citizenstate);
	void Remove_Citizen(int citizen_id);
	void Spawn_Army(void *packet);
	void Set_Army_Location(int a_id, FVector Location, FRotator Rotate, char a_state);
	void Set_Army_Disband(int a_id);
};
