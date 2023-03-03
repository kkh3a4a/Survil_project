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
        //UE_LOG(LogTemp, Log, TEXT("%d"), TerrainTemperaturePtr[i * 4 % (MapSizeX / 4)][i * 4 / (MapSizeX / 4)]);
        TemperatureToRGB(TerrainTemperaturePtr[i * 4 % (MapSizeX / 4)][i * 4 / (MapSizeX / 4)], RGB.X, RGB.Y, RGB.Z);
        MaterialInstanceArray[i]->SetVectorParameterValue(TEXT("Temperature"), FLinearColor(RGB.X, RGB.Y, RGB.Z, 1));
		DecalActor->SetDecalMaterial(MaterialInstanceArray[i]);
	}
}

void ATemperature::TemperatureToRGB(double temperature, double& r, double& g, double& b) {
    double scaledTemperature = temperature / 100.0;

    // Calculate the red, green, and blue components
    r = std::max(0.0, std::min(1.0, 2.0 - scaledTemperature * 2.0));
    g = std::max(0.0, std::min(1.0, scaledTemperature * 2.0));
    b = std::max(0.0, std::min(1.0, 2.0 * (scaledTemperature - 0.5)));

    // Normalize the RGB values so their sum is 1
    double sum = r + g + b;
    if (sum > 0) {
        r /= sum;
        g /= sum;
        b /= sum;
    }
}