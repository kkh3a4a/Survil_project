// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTown.h"

// Sets default values
AMyTown::AMyTown()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyTown::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyTown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyTown::Initialize(TSubclassOf<AActor> well, TSubclassOf<AActor> oil, TSubclassOf<AActor> water, TSubclassOf<AActor> iron, TSubclassOf<AActor>food, TSubclassOf<AActor>wood)
{
	WellPump = well;
	OilActor = oil;
	WaterActor = water;
	IronActor = iron;
	FoodActor = food;
	WoodActor = wood;
}

void AMyTown::SpawnTown(TMap<int, FPlayerInfo*>& Players)
{
	AActor* SpawnedTown;
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	for (int i = 0; i < MAXPLAYER; ++i)
	{
		Location = { (double)Players[i]->CityLocation.x, (double)Players[i]->CityLocation.y, 0.f};
		SpawnedTown = GetWorld()->SpawnActor<AActor>(WellPump, Location, Rotation, SpawnInfo);
		SpawnedTown->Tags.Add("WellPump");
		SpawnedTown->Tags.Add(FName(*FString::FromInt(i)));
	}
}

void AMyTown::SpawnResource(TMap<int, FPlayerInfo*>& Players)
{
	AActor* SpawnedResource;
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	for (int i = 0; i < MAXPLAYER * 5; ++i)
	{
		resources_create_landscape.Add(&Players[0]->Resource[i]);
		Location = { (double)resources_create_landscape[i]->Location.x,  (double)resources_create_landscape[i]->Location.y, 0 };
		
		if (resources_create_landscape[i]->Type == 0)
		{
			SpawnedResource = GetWorld()->SpawnActor<AActor>(OilActor, Location, Rotation, SpawnInfo);
			SpawnedResource->Tags.Add("Resource");
			SpawnedResource->Tags.Add("0");
		}
		else if (resources_create_landscape[i]->Type == 1)
		{
			SpawnedResource = GetWorld()->SpawnActor<AActor>(WaterActor, Location, Rotation, SpawnInfo);
			SpawnedResource->Tags.Add("Resource");
			SpawnedResource->Tags.Add("1");
		}
		else if (resources_create_landscape[i]->Type == 2)
		{
			SpawnedResource = GetWorld()->SpawnActor<AActor>(IronActor, Location, Rotation, SpawnInfo);
			SpawnedResource->Tags.Add("Resource");
			SpawnedResource->Tags.Add("2");
		}
		else if (resources_create_landscape[i]->Type == 3)
		{
			SpawnedResource = GetWorld()->SpawnActor<AActor>(FoodActor, Location, Rotation, SpawnInfo);
			SpawnedResource->Tags.Add("Resource");
			SpawnedResource->Tags.Add("3");
		}
		else if (resources_create_landscape[i]->Type == 4)
		{
			SpawnedResource = GetWorld()->SpawnActor<AActor>(WoodActor, Location, Rotation, SpawnInfo);
			SpawnedResource->Tags.Add("Resource");
			SpawnedResource->Tags.Add("4");
		}
		else
		{
			SpawnedResource = GetWorld()->SpawnActor<AActor>(WoodActor, Location, Rotation, SpawnInfo);
			SpawnedResource->Tags.Add("Resource");
			SpawnedResource->Tags.Add("5");
		}
		SpawnedResource->Tags.Add(FName(*FString::FromInt(i)));
		ResourceEditer.Add(SpawnedResource);
	}
}

void AMyTown::UpdateResource()
{
	for (int resource_checking = 0; resource_checking < MAXPLAYER * 5; ++resource_checking)
	{
		if (resources_create_landscape[resource_checking]->Count == 0)
		{
					ResourceEditer[resource_checking]->SetActorHiddenInGame(true);
					ResourceEditer[resource_checking]->SetActorEnableCollision(false);
		}
	}
}
