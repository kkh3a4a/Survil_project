// Fill out your copyright notice in the Description page of Project Settings.

#include "Async/ParallelFor.h"
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

    for (int i = 0; i < (SIGHT_X / ColorsInDecalX) * (SIGHT_Y / ColorsInDecalY); i++) {
        UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(TemperatureMaterial, this);
        MaterialInstanceArray.Add(MaterialInstance);
    }

    FVector Location(0, 0, 0);
    FRotator Rotation = FRotator(0, 0, 0);
    FActorSpawnParameters SpawnInfo;

    for (int32 y = 0; y < (SIGHT_Y / ColorsInDecalY); y++) {
        for (int32 x = 0; x < (SIGHT_X / ColorsInDecalX); x++) {
            ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(Location + FVector(2000 * x + 1000, 2000 * y + 1000, 0), Rotation);
            UDecalComponent* DecalComponent = Decal->GetDecal();
            DecalComponent->DecalSize = FVector(100, 100, 100);
            Decal->SetActorScale3D(FVector(10, _DecalSize, _DecalSize));
            Decal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            Decal->SetDecalMaterial(MaterialInstanceArray[y * (SIGHT_X / ColorsInDecalX) + x]);
            DecalArray.Add(Decal);
        }
    }
    Hide(true);

    //TerrainTemperature32.SetNum(SIGHT_X);
}

void ATemperature::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsHidden)
        return;

    FVector RGB;
    for (int y = 0; y < SIGHT_Y; y++) {
        for (int x = 0; x < SIGHT_X; x++) {
            TemperatureToRGB(TerrainTemperature[x][y], RGB.X, RGB.Y, RGB.Z);
            MaterialInstanceArray[(y / 20) * (SIGHT_X / ColorsInDecalX) + (x / 20)]->SetVectorParameterValue(*FString::Printf(TEXT("Color%d"), (x % 20) * 20 + (y % 20)), FLinearColor(RGB.X, RGB.Y, RGB.Z, 1));
        }
    }
    
    
    //for (int y = 0; y < SIGHT_Y; y++) {
    //    TFunction<void(int32)> MyLambda = [&](int32 Index)
    //    {
    //        FVector RGB;
    //        uint8 _Temperature = TerrainTemperature[Index][y];
    //        TemperatureToRGB(_Temperature, RGB.X, RGB.Y, RGB.Z);
    //        if ((y / 20) * 12 + (Index / 20) >= 60 || (Index % 20) * 20 + (y % 20) >= 400) {
    //            UE_LOG(LogTemp, Warning, TEXT("%d of %d,  %d of 400"), (y / 20) * 12 + (Index / 20), MaterialInstanceArray.Num(), (Index % 20) * 20 + (y % 20));
    //        }
    //        //MaterialInstanceArray[(y / 20) * 12 + (Index / 20)]->SetVectorParameterValue(*FString::Printf(TEXT("Color%d"), (Index % 20) * 20 + (y % 20)), FLinearColor(RGB.X, RGB.Y, RGB.Z, 1));
    //    };
    //    ParallelFor(TerrainTemperature32.Num(), MyLambda);
    //}
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
