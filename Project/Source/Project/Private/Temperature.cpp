// Fill out your copyright notice in the Description page of Project Settings.


#include "Temperature.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ATemperature::ATemperature()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Set the root component as the root of the actor
	RootComponent = Root;
}

void ATemperature::Initiaize(UMaterialInstance* Material)
{
    const int32 numRows = 200;
    const int32 numCols = 120;
    const float decalWidth = 1.0f;
    const float decalHeight = 1000.0f;

    for (int32 row = 0; row < numRows; row++)
    {
        for (int32 col = 0; col < numCols; col++)
        {
            // Calculate the spawn location for this decal
            FVector spawnLocation = FVector(row * decalWidth, col * decalHeight, 0.0f);

            // Spawn the decal at this location
            TemperatureDecal = UGameplayStatics::SpawnDecalAtLocation(
                GetWorld(),
                Material,
                FVector(decalWidth, decalHeight, 1.0f),
                spawnLocation,
                FRotator::ZeroRotator,
                10
            );
            // Set other properties of the decal
            /*TemperatureDecal->SetFadeOut(10, 1, 1, false);
            TemperatureDecal->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));*/
        }
    }
}

void ATemperature::Update()
{

}
