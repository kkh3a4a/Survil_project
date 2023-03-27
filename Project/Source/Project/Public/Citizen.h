// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"global.h"
#include "GameFramework/Actor.h"
#include "Citizen.generated.h"


UCLASS()
class PROJECT_API ACitizen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACitizen();
	void Initialize(TSubclassOf<AActor>, TSubclassOf<AActor>);

protected:

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Citizen_Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor>EnemyCitizenActor;

	FCitizenMoving* Citizen_moving = new FCitizenMoving;
	int citizen_cnt[MAXPLAYER];
	TMap<int, FCitizenSole*> My_Citizen;
	TMap<int, TArray<AActor*>> Citizens_Editer;
	
	int My_Citizen_Num = 0;
	void Spawn_Citizen();
	void Citizen_Moving();
	void CitizenSet(FCitizenSole(*Citizens));
	void TF_set(FThreeFloat& a, FThreeFloat& b);
	void CitizenNoJob(int& cnt);
};
