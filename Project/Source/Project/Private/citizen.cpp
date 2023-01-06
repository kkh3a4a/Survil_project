// Fill out your copyright notice in the Description page of Project Settings.


#include "citizen.h"

// Sets default values
Acitizen::Acitizen()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Acitizen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Acitizen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Acitizen::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

