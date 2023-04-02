// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABuilding::ABuilding()
{
	
}

void ABuilding::Initialize(UMaterial* Material, TArray<TSubclassOf<AActor>*> Buildings)
{
	FVector Location = FVector(0, 0, 0);
	DecalActor = GetWorld()->SpawnActor<ADecalActor>(Location, FRotator(0.f, -90.f, 0.f));
	DecalActor->SetActorScale3D(FVector(10.f, 2.f, 2.f));

	UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);

	DecalActor->SetDecalMaterial(MaterialInstance);
	DecalActor->SetActorHiddenInGame(true);

	BuildingArray = Buildings;
}

void ABuilding::DecalVisibility()
{
	if (BuildMode) {
		DecalActor->SetActorHiddenInGame(false);
	}
	else {
		DecalActor->SetActorHiddenInGame(true);
	}
	SelectedBuilding = 0;
}

void ABuilding::Build()
{
	if (SelectedBuilding == 0) {
		UE_LOG(LogTemp, Log, TEXT("Select Type of Building!!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Selected Building %d"), SelectedBuilding);
	FRotator Rotation = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo;
	AActor* Building;
	Building = GetWorld()->SpawnActor<AActor>(*BuildingArray[SelectedBuilding - 1], DecalLocation, Rotation, SpawnInfo);
	BuildedBuildings.Add(Building);
	UE_LOG(LogTemp, Log, TEXT("Builded Buildings: %d"), BuildedBuildings.Num());

}

void ABuilding::Update()
{
	DecalActor->SetActorLocation(DecalLocation);
}
