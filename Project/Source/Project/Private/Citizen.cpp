// Fill out your copyright notice in the Description page of Project Settings.


#include "Citizen.h"

// Sets default values
ACitizen::ACitizen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACitizen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACitizen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

