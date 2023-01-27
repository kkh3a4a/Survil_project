// Fill out your copyright notice in the Description page of Project Settings.

#include <math.h>
#include "servers_sand_test.h"

// Sets default values
Aservers_sand_test::Aservers_sand_test()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	for (int i = 0; i < 100; ++i)
		IntArray.Emplace(i);

	sands_size = sqrt(IntArray.Num());

	UE_LOG(LogTemp, Warning, TEXT("%d "), sands_size);
}

// Called when the game starts or when spawned
void Aservers_sand_test::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Aservers_sand_test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

