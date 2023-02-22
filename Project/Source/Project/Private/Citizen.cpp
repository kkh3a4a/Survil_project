// Fill out your copyright notice in the Description page of Project Settings.


#include "Citizen.h"

// Sets default values
ACitizen::ACitizen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}
void ACitizen::Initialize(TSubclassOf<AActor> C_Actor)
{
	Citizen_Actor = C_Actor;
}

void ACitizen::Spawn_Citizen()
{
	AActor* Spawned_Citizen;
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	TArray<AActor*> New_Actors;
	int Is_Mycitizen = 0;
	for (auto& a : My_Citizen)
	{
		if (Is_Mycitizen == 0)
		{
			for (int i = 0; i < 10; ++i)
			{
				Location = { a.Value.citizen_location_rotation[i].location.x, a.Value.citizen_location_rotation[i].location.y, a.Value.citizen_location_rotation[i].location.z };
				//Spawned_Citizen = GetWorld()->SpawnActor<AActor>(Location, Rotation, SpawnInfo);
				Spawned_Citizen = GetWorld()->SpawnActor<AActor>(Citizen_Actor, Location, Rotation, SpawnInfo);
				New_Actors.Add(Spawned_Citizen);
			}
			Citizens_Editer.Add(Is_Mycitizen, New_Actors);
		}
		else
		{
			for (int i = 0; i < 10; ++i)
			{
				Location = { a.Value.citizen_location_rotation[i].location.x, a.Value.citizen_location_rotation[i].location.y, a.Value.citizen_location_rotation[i].location.z };
				GetWorld()->SpawnActor<ACitizen>(Location, Rotation, SpawnInfo);
			}
		}
		Is_Mycitizen++;
	}
}

void ACitizen::Citizen_Moving()
{
	int team = 0;
	FVector Location(0.0f, 0.0f, 0.0f);
	for (auto& a : My_Citizen)
	{
		for (int i = 0; i < a.Value.citizen_location_rotation.Num(); ++i)
		{
			Location = { a.Value.citizen_location_rotation[i].location.x,a.Value.citizen_location_rotation[i].location.y,a.Value.citizen_location_rotation[i].location.z };
			Citizens_Editer[team][i]->SetActorLocation(Location);
		}
		team++;
	}

}


