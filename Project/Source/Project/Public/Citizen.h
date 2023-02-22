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
	void Initialize(TSubclassOf<AActor>);

protected:

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Citizen_Actor;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//FActor_location_rotation MouseInput;

		FCitizen_moving Citizen_moving;

		int citizen_cnt[MAXPLAYER];

		FCitizen_sole temp_Actor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int, Fcitizen_struct> My_Citizen;

		TMap<int, TArray<AActor*>> Citizens_Editer;
	


	int My_Citizen_Num = 0;


	void Spawn_Citizen();

	void Citizen_Moving();
};
