// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameEvent.generated.h"

UCLASS()
class PROJECT_API AGameEvent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameEvent();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	

	int e_id = 0;
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	void set_event_id(int event_id);

};
