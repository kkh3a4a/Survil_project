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
	float _x, _y, _z;
	char job;

	void Spawn_Citizen();

	void Citizen_Moving();
};
