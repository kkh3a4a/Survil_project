// Fill out your copyright notice in the Description page of Project Settings.


#include "Sunlight.h"

// Sets default values
ASunlight::ASunlight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASunlight::BeginPlay()
{
	Super::BeginPlay();
	SunAngle = 180.0f;
	SunAngleValue = 0.2f;

}

// Called every frame
void ASunlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChangeRotation();
}

void ASunlight::ChangeRotation()
{
	if (SunAngle >= 360)
	{
		SunAngle = 0;
	}
	SunAngle += SunAngleValue;
	NewRotation = FRotator(SunAngle, 0, 0);


	FQuat QuatRotation = FQuat(NewRotation);

	SetActorRotation(QuatRotation, ETeleportType::None);
}

double ASunlight::RtnSunAngle()
{
	return SunAngle;
}