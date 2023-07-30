// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Citizen.h"
#include "Army.h"
#include<concurrent_queue.h>
#include"../../../../IOCPServer/protocol.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>ARMY_MODEL1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>ARMY_MODEL2;

	UPROPERTY(EditAnywhere)
	class AMain* Main;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AActor* citizen[1000];
	AActor* army[100];
	class FSocketThread* Network;
	void Spawn_Citizen();
	void Set_Citizen_Location(int citizen_id, FVector Location, FRotator Rotate, char citizenstate);
	void Remove_Citizen(int citizen_id);
	void Spawn_Army(sc_packet_armytraining spawn_packet);
	void Set_Army_Location(int a_id, FVector Location, FRotator Rotate, char a_state);
	void Set_Army_Disband(int a_id);
	void Set_Army_Hp(int Hp, int a_id);
	void Army_Dead(int a_id);
	void Set_Army_Attack(int a_id, FRotator Rotate, int a_state);
	void Set_Army_Queue(void* packet);
	void Set_Citizen_Move_Queue(sc_packet_citizenmove* packet);

	//1000이라서 부하가 엄청 걸릴지도 모르겠다
	int CitizensToWaitForSpawn[1000]{};
	FVector CitizensForSpawnLocation[1000]{};
	void PutCitizenForSpawn(int id, FVector location);

	int ArmiesToWaitForSpawn[1000]{};
	FVector ArmiesForSpawnLocation[1000]{};
	void PutArmyForSpawn(int id, FVector location);
	
	
	concurrency::concurrent_queue<sc_packet_armytraining> Army_Queue;
	concurrency::concurrent_queue <sc_packet_citizenmove> Citizen_Move_Queue;
};
