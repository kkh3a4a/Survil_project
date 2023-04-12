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

// ATemperature::Initiaize(UMaterial* Material)
void ATemperature::Initiaize(UMaterial* Material)
{
    /*const float Width = 200; 
    const float Height = 5000;

    for (int32 y = 0; y < MapSizeY / 2; y++){
        for (int32 x = 0; x < MapSizeX / 2; x++){
            DecalActor = GetWorld()->SpawnActor<ADecalActor>(FVector(Width * x + Width / 2, Width * y + Width / 2, 0), FRotator(0,-90,0));
			DecalActor->SetActorScale3D(FVector(Height, 2, 2));
			DecalActor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            
            UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
            MaterialInstance->SetVectorParameterValue(TEXT("Temperature"), FLinearColor(1, 0, 0, 1));
            MaterialInstanceArray.Add(MaterialInstance);
			
            DecalActor->SetDecalMaterial(MaterialInstance);
			DecalArray.Add(DecalActor);
        }
    }
	Hide(true);*/

    MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
    for (int i = 0; i < 12 * 5; i++) {
        MaterialInstanceArray.Add(MaterialInstance);
    }

    FVector Location(0, 0, 0);
    FRotator Rotation = FRotator(0, 0, 0);
    FActorSpawnParameters SpawnInfo;


    for (int32 y = 0; y < 5; y++) {
        for (int32 x = 0; x < 12; x++) {
            ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(Location + FVector(4000 * x, 4000 * y, 0), Rotation);
            UDecalComponent* DecalComponent = Decal->GetDecal();
            DecalComponent->DecalSize = FVector(100, 100, 100);
            Decal->SetActorScale3D(FVector(10, _DecalSize, _DecalSize));
            Decal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            Decal->SetDecalMaterial(MaterialInstanceArray[y*12+x]);
            DecalArray.Add(Decal);
        }
    }
    Hide(true);
}

void ATemperature::Update(uint8(*TerrainTemperaturePtr)[MapSizeY])
{
	if (IsHidden)
		return;
	for (int32 i = 0; i < 12*5; i++) {
        FVector RGB;
        TemperatureToRGB(TerrainTemperaturePtr[i % MapSizeX][i / MapSizeX], RGB.X, RGB.Y, RGB.Z);
        //UE_LOG(LogTemp, Warning, TEXT("%d %d"), (i) % (MapSizeX / 2), (i) / (MapSizeX / 2));
        //MaterialInstanceArray[i]->SetVectorParameterValue(*FString::Printf(TEXT("Color%d"), i), FLinearColor(RGB.X, RGB.Y, RGB.Z, 1));
        for(int c = 0; c < 20*20; c++)
            MaterialInstanceArray[i]->SetVectorParameterValue(*FString::Printf(TEXT("Color%d"), c), FLinearColor((float)(FMath::RandRange(0, 100)) / 100.f, (float)(FMath::RandRange(0, 100)) / 100.f, (float)(FMath::RandRange(0, 100)) / 100.f, 1));
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
    if (r && b) {
    }
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
