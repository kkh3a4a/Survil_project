// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceManager.h"
#include "Main.h"
#include "NetworkingThread.h"


// Sets default values
AResourceManager::AResourceManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AResourceManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AResourceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (Network == nullptr)
    {
        Network = reinterpret_cast<FSocketThread*>(Main->Network);
        Network->_ResourceManager = this;
    }
    for (int i = 0; i < 50; ++i)
    {
        if (resources[i] != nullptr)
        {
            if (resource_amount[i] == 0)
            {
                resources[i]->SetActorHiddenInGame(true);
                resources[i]->SetActorEnableCollision(false);
            }
        }
    }
}

void AResourceManager::Spawn_Resource(int Resource_id, FVector Location, int amount , char resourcetype)
{
    FActorSpawnParameters SpawnInfo;
    if (resourcetype == 0)
        resources[Resource_id] = GetWorld()->SpawnActor<AActor>(OilActor, Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
    else if (resourcetype == 1)
        resources[Resource_id] = GetWorld()->SpawnActor<AActor>(WaterActor, Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
    else if (resourcetype == 2)
        resources[Resource_id] = GetWorld()->SpawnActor<AActor>(IronActor, Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
    else if (resourcetype == 3)
        resources[Resource_id] = GetWorld()->SpawnActor<AActor>(FoodActor, Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
    else if (resourcetype == 4)
        resources[Resource_id] = GetWorld()->SpawnActor<AActor>(WoodActor, Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
    else
        return;
    //tagÃß°¡ 0 : resource , 1 : Resource_id
   // AActor* SpawnedResource = resources[Resource_id];
    resources[Resource_id]->Tags.Add("Resource");
    resources[Resource_id]->Tags.Add(FName(*FString::FromInt(Resource_id)));
    
    resource_amount[Resource_id] = amount;
    resource_type[Resource_id] = resourcetype;
}

void AResourceManager::SetResourceAmount(int Resource_id, int amount)
{
    resource_amount[Resource_id] = amount;
}

void AResourceManager::SetResourcePlacement(int Resource_id, char work_citizen)
{
    workCitizens[Resource_id] = work_citizen;
}

void AResourceManager::Spawn_Event(int e_id, FVector Location)
{
    FActorSpawnParameters SpawnInfo;
    GameEvent[e_id] = GetWorld()->SpawnActor<AActor>(GameEventActor, Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
}

void AResourceManager::remove_Event(int e_id)
{
}

