// Fill out your copyright notice in the Description page of Project Settings.


#include "Temperature.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATemperature::ATemperature()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;
}

void ATemperature::BeginPlay()
{
    Super::BeginPlay();

    for (int i = 0; i < 12 * 5; i++) {
        UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(TemperatureMaterial, this);
        MaterialInstanceArray.Add(MaterialInstance);
    }

    FVector Location(0, 0, 0);
    FRotator Rotation = FRotator(0, 0, 0);
    FActorSpawnParameters SpawnInfo;

    for (int32 y = 0; y < 5; y++) {
        for (int32 x = 0; x < 12; x++) {
            ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(Location + FVector(2000 * x + 1000, 2000 * y + 1000, 0), Rotation);
            UDecalComponent* DecalComponent = Decal->GetDecal();
            DecalComponent->DecalSize = FVector(100, 100, 100);
            Decal->SetActorScale3D(FVector(10, _DecalSize, _DecalSize));
            Decal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            Decal->SetDecalMaterial(MaterialInstanceArray[y * 12 + x]);
            DecalArray.Add(Decal);
        }
    }
    Hide(true);
}

void ATemperature::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsHidden)
        return;
    FVector RGB;
    //for (int32 y = 0; y < 5; y++) {
    //    for (int32 x = 0; x < 12; x++) {
    //        for (int cy = 0; cy < 20; cy++) {
    //            for (int cx = 0; cx < 20; cx++) {
    //                TemperatureToRGB(TerrainTemperature[x * cx][y * cy], RGB.X, RGB.Y, RGB.Z);
    //                //MaterialInstanceArray[i]->SetVectorParameterValue(*FString::Printf(TEXT("Color%d"), c), FLinearColor((float)(FMath::RandRange(0, 100)) / 100.f, (float)(FMath::RandRange(0, 100)) / 100.f, (float)(FMath::RandRange(0, 100)) / 100.f, 1));
    //                MaterialInstanceArray[y * 5 + x]->SetVectorParameterValue(*FString::Printf(TEXT("Color%d"), cy * 20 + cx), FLinearColor(RGB.X, RGB.Y, RGB.Z, 1));
    //            }
    //        }
    //    }
    //}
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 240; x++) {
            TemperatureToRGB(TerrainTemperature[x][y], RGB.X, RGB.Y, RGB.Z);
            MaterialInstanceArray[(y / 20) * 12 + (x / 20)]->SetVectorParameterValue(*FString::Printf(TEXT("Color%d"), (x % 20) * 20 + (y % 20)), FLinearColor(RGB.X, RGB.Y, RGB.Z, 1));

        }
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
