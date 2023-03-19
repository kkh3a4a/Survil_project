// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABuilding::ABuilding()
{
	
}

void ABuilding::Initialize(UMaterial* Material)
{
	FVector Location = FVector(0, 0, 0);
	DecalActor = GetWorld()->SpawnActor<ADecalActor>(Location, FRotator(0.f, -90.f, 0.f));
	DecalActor->SetActorScale3D(FVector(10.f, 2.f, 2.f));

	UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);

	DecalActor->SetDecalMaterial(MaterialInstance);
}

void ABuilding::update()
{
	DecalActor->SetActorLocation(DecalLocation);
}
