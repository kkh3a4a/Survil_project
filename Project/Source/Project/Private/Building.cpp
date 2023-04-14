// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "Kismet/GameplayStatics.h"
#define CITYSIZE 100

// Sets default values
ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();

}

void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABuilding::SetMesh(TSubclassOf<AActor> Mesh,int type, int id)
{
	FActorSpawnParameters SpawnInfo;
	AActor* BuildingMesh = GetWorld()->SpawnActor<AActor>(Mesh, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnInfo);
	BuildingMesh->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	BuildingMesh->Tags.Add(TEXT("Building"));
	BuildingMesh->Tags.Add(*FString::FromInt(type));
	BuildingMesh->Tags.Add(*FString::FromInt(id));
}
