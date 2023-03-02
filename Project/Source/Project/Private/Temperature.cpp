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

void ATemperature::Initiaize(UMaterial* Material)
{
    
    
    const float Width = 4 * 100;
    const float Height = 5000;

    for (int32 row = 0; row < MapSizeX / 4; row++){
        for (int32 col = 0; col < MapSizeY / 4; col++){
            DecalActor = GetWorld()->SpawnActor<ADecalActor>(FVector(Width * row + Width/2, Width * col + Width / 2, 0), FRotator(0,-90,0));
			DecalActor->SetActorScale3D(FVector(Height, 2, 2));
			DecalActor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            
            UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
            MaterialInstance->SetVectorParameterValue(TEXT("Temperature"), FLinearColor(1, 0, 0, 1));
            MaterialInstanceArray.Add(MaterialInstance);
			
            DecalActor->SetDecalMaterial(MaterialInstance);
			DecalArray.Add(DecalActor);
        }
    }
}

void ATemperature::Update(int8(*TerrainTemperaturePtr)[MapSizeY])
{
	for (int32 i = 0; i < DecalArray.Num(); i++) {
        FVector RGB;
        CelsiusToRGB(TerrainTemperaturePtr[i * 4 % (MapSizeX / 4)][i * 4 / (MapSizeX / 4)], RGB.X, RGB.Y, RGB.Z);
        MaterialInstanceArray[i]->SetVectorParameterValue(TEXT("Temperature"), FLinearColor(RGB.X, RGB.Y, RGB.Z, 1));
		DecalActor->SetDecalMaterial(MaterialInstanceArray[i]);
	}
}

void ATemperature::CelsiusToRGB(double celsius, double& r, double& g, double& b) {
    // Scale the temperature value from 0 to 1
    double t = std::min(std::max(celsius / 100.0, 0.0), 1.0);

    // Calculate the RGB values based on the temperature value
    if (t <= 0.5) {
        r = t * 2.0;
        g = 0.0;
        b = (1.0 - t) * 2.0;
    }
    else {
        r = (1.0 - t) * 2.0;
        g = (t - 0.5) * 2.0;
        b = 0.0;
    }
}