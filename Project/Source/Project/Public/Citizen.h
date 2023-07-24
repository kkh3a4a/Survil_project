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
	ACitizen(int id);
protected:

public:
	int _id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float _rx;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float _ry;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float _rz;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator CitizenRotation;

	char job;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	uint8 state;

	void Spawn_Citizen();

};
