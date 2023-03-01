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
    UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
	MaterialInstance->SetVectorParameterValue(TEXT("Temperature"), FLinearColor(1, 0, 0, 1));
    
    const float Width = 1000;
    const float Height = 5000;

    for (int32 row = 0; row < 20; row++)
    {
        for (int32 col = 0; col < 12; col++)
        {
            DecalActor = GetWorld()->SpawnActor<ADecalActor>(FVector(Width * row + Width/2, Width * col + Width / 2, 0), FRotator(0,-90,0));
			DecalActor->SetActorScale3D(FVector(Height, 3, 3));
			DecalActor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			DecalActor->SetDecalMaterial(MaterialInstance);
			DecalArray.Add(DecalActor);
        }
    }
}

void ATemperature::Update()
{

    
}
