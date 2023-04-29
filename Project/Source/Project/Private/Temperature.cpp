// Fill out your copyright notice in the Description page of Project Settings.

#include "Temperature.h"
#include "Async/ParallelFor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATemperature::ATemperature()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;
}

ATemperature::~ATemperature()
{
	if (Work != nullptr)
		Work->Stop();
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
            ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(FVector(2000 * x + 1000, 2000 * y + 1000, 0), Rotation);
            UDecalComponent* DecalComponent = Decal->GetDecal();
			DecalComponent->DecalSize = FVector(100, 100, 100);
            Decal->SetActorScale3D(FVector(10, _DecalSize, _DecalSize));
            Decal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            Decal->SetDecalMaterial(MaterialInstanceArray[y * (SIGHT_X / ColorsInDecalX) + x]);
            DecalArray.Add(Decal);
        }
    }
    Hide(true);

	Work = new FTemperatureThread();
	Work->TemperatureClass = this;
	WorkThread = FRunnableThread::Create(Work, TEXT("TemperatureThread"), 0, TPri_BelowNormal);
}

void ATemperature::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsHidden)
        return;
	if (!ReadyToUpdate) {
		return;
	}

	FVector RGB;

	for (int y = 0; y < SIGHT_Y; y++) {
		for (int x = 0; x < SIGHT_X; x++) {
			TemperatureToRGB(TerrainTemperature[x][y], RGB.X, RGB.Y, RGB.Z);
			MaterialInstanceArray[(y / 20) * (SIGHT_X / ColorsInDecalX) + (x / 20)]->SetVectorParameterValue(*FString::Printf(TEXT("Color%d"), (x % 20) * 20 + (y % 20)), FLinearColor(RGB.X, RGB.Y, RGB.Z, 1));
		}
	}

	ReadyToUpdate = false;
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
	if (!visibility) {
		ReadyToUpdate = true;
	}
	for (ADecalActor* Decal : DecalArray) {
        Decal->SetActorHiddenInGame(visibility);
	}
	IsHidden = visibility;
}

bool ATemperature::GetIsHidden()
{
    return IsHidden;
}

FTemperatureThread::FTemperatureThread()
{
}

void FTemperatureThread::Stop()
{
	Running = false;
}

uint32_t FTemperatureThread::Run()
{
	UE_LOG(LogTemp, Warning, TEXT("Temperature Thread Start"));
	while (1) {
		if (!Running)
			break;
		/*if (TemperatureClass->GetIsHidden()) {
			FPlatformProcess::Sleep(0.1f);
			continue;
		}*/

		if (LineX) {
			static float TerrainX = x;
			if ((int)TerrainX != (int)x) {
				if ((int)TerrainX < (int)x) {
					memcpy(TemperatureClass->TerrainTemperature[0], TemperatureClass->TerrainTemperature[1], SIGHT_Y * (SIGHT_X - 1));
					memcpy(TemperatureClass->TerrainTemperature[SIGHT_X - 1], TerrainLineY, SIGHT_Y);
				}
				if ((int)TerrainX > (int)x) {
					memcpy(TemperatureClass->TerrainTemperature[1], TemperatureClass->TerrainTemperature[0], SIGHT_Y * (SIGHT_X - 1));
					memcpy(TemperatureClass->TerrainTemperature[0], TerrainLineY, SIGHT_Y);
				}
				TerrainX = x;
			}
			LineX = false;
			TemperatureClass->ReadyToUpdate = true;
		}
		if (LineY) {
			static float TerrainY = y;
			if ((int)TerrainY != (int)y) {
				if ((int)TerrainY < (int)y) {
					for (int i = 0; i < SIGHT_X - 1; ++i) {
						for (int j = 0; j < SIGHT_Y - 1; ++j)
							TemperatureClass->TerrainTemperature[i][j] = TemperatureClass->TerrainTemperature[i][j + 1];
					}
					for (int i = 0; i < SIGHT_X; ++i) {
						TemperatureClass->TerrainTemperature[i][SIGHT_Y - 1] = TerrainLineX[i];
					}
				}
				if ((int)TerrainY > (int)y) {
					for (int i = 0; i < SIGHT_X - 1; ++i) {
						for (int j = SIGHT_Y - 1; j > 0; --j) {
							TemperatureClass->TerrainTemperature[i][j] = TemperatureClass->TerrainTemperature[i][j - 1];
						}
					}
					for (int i = 0; i < SIGHT_X; ++i) {
						TemperatureClass->TerrainTemperature[i][0] = TerrainLineX[i];
					}
				}
				TerrainY = y;
			}
			LineY = false;
			TemperatureClass->ReadyToUpdate = true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Temperature Thread Dead"));
	return 0;
}
