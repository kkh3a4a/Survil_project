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

void AMyTown::SpawnTown(TMap<int, FActor_location_rotation>& player_list)
{
	AActor* SpawnedTown;
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	for (int i = 0; i < MAXPLAYER; ++i)
	{
		Location = { player_list[i].location.x,player_list[i].location.y,player_list[i].location.z};
		SpawnedTown = GetWorld()->SpawnActor<AActor>(WellPump, Location, Rotation, SpawnInfo);
		SpawnedTown->Tags.Add("WellPump");
		SpawnedTown->Tags.Add(FName(*FString::FromInt(i)));
	}

}

void AMyTown::SpawnResource()
{
	AActor* SpawnedResource;
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	for (int i = 0; i < MAXPLAYER * 10; ++i)
	{
		Location = { resources_create_landscape[i].location.x,resources_create_landscape[i].location.y,resources_create_landscape[i].location.z };

		if (resources_create_landscape[i].type == 0)
		{
			SpawnedResource = GetWorld()->SpawnActor<AActor>(OilActor, Location, Rotation, SpawnInfo);
			SpawnedResource->Tags.Add("Resource");
			SpawnedResource->Tags.Add("0");
		}
		else if (resources_create_landscape[i].type == 1)
		{
			SpawnedResource = GetWorld()->SpawnActor<AActor>(WaterActor, Location, Rotation, SpawnInfo);
			SpawnedResource->Tags.Add("Resource");
			SpawnedResource->Tags.Add("1");
		}
		else if (resources_create_landscape[i].type == 2)
		{
			SpawnedResource = GetWorld()->SpawnActor<AActor>(IronActor, Location, Rotation, SpawnInfo);
			SpawnedResource->Tags.Add("Resource");
			SpawnedResource->Tags.Add("2");
		}
		else if (resources_create_landscape[i].type == 3)
		{
			SpawnedResource = GetWorld()->SpawnActor<AActor>(FoodActor, Location, Rotation, SpawnInfo);
			SpawnedResource->Tags.Add("Resource");
			SpawnedResource->Tags.Add("3");
		}
		else if (resources_create_landscape[i].type == 4)
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

void AMyTown::UpdateResource(Fresources_actor& a, Fresources_actor& b, int i)
{
	a.count = b.count;
	a.type = b.type;
	a.CitizenCount = b.CitizenCount;
	if (a.count == 0)
	{
		ResourceEditer[i]->SetActorHiddenInGame(true);
	}
}

