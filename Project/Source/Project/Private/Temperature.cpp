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
    const float Width = 100 * divide;
    const float Height = 5000;

    for (int32 y = 0; y < MapSizeY / divide; y++){
        for (int32 x = 0; x < MapSizeX / divide; x++){
            DecalActor = GetWorld()->SpawnActor<ADecalActor>(FVector(Width * x + Width/2, Width * y + Width / 2, 0), FRotator(0,-90,0));
			DecalActor->SetActorScale3D(FVector(Height, divide, divide));
			DecalActor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            
            UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
            MaterialInstance->SetVectorParameterValue(TEXT("Temperature"), FLinearColor(1, 0, 0, 1));
            MaterialInstanceArray.Add(MaterialInstance);
			
            DecalActor->SetDecalMaterial(MaterialInstance);
			DecalArray.Add(DecalActor);
        }
    }
	Hide(true);
}

void ATemperature::Update(uint8(*TerrainTemperaturePtr)[MapSizeY])
{
	if (IsHidden)
		return;
	for (int32 i = 0; i < MapSizeX / divide * MapSizeY / divide; i++) {
        FVector RGB;
		//UE_LOG(LogTemp, Warning, TEXT("%d %d %d"), TerrainTemperaturePtr[(i * divide) % MapSizeX][(i * divide) / MapSizeX * divide],(i * divide) % MapSizeX, (i * divide) / MapSizeX * divide);
        TemperatureToRGB(TerrainTemperaturePtr[(i * divide) % MapSizeX][(i * divide) / MapSizeX * divide], RGB.X, RGB.Y, RGB.Z);
        MaterialInstanceArray[i]->SetVectorParameterValue(TEXT("Temperature"), FLinearColor(RGB.X, RGB.Y, RGB.Z, 1));
	}
}

void ATemperature::TemperatureToRGB(double temperature, double& r, double& g, double& b) {
    double scaledTemperature = temperature / TemperatureDivide - 20; //20 ~ 60 -> 0 ~ 40

    // rgbÃÑÇÕ 20
    if (scaledTemperature > 20) {
		r = scaledTemperature - 20;
        g = 20 - r;
        b = 0;
    }
    else {
        r = 0;
        g = scaledTemperature;
        b = 20 - g;
    }

    r /= 20;
	g /= 20;
	b /= 20;

    //UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), r, g, b);
}

void ATemperature::Hide(bool visibility)
{
	for (ADecalActor* Decal : DecalArray) {
        Decal->SetActorHiddenInGame(visibility);
	}
	IsHidden = visibility;
}


bool ATemperature::GetIsHidden()
{
    return IsHidden;
}
