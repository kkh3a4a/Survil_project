// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayersStep.h"

// Sets default values
APlayersStep::APlayersStep()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HopePercent = FMath::Clamp(HopePercent, 0.0f, 1.0f);
	ComplaintPercent = FMath::Clamp(ComplaintPercent, 0.0f, 1.0f);
}

// Called when the game starts or when spawned
void APlayersStep::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayersStep::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

