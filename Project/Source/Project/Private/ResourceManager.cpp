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
    s_resource create_resource;
    while(!resourceCreateQueue.empty())
    {
        if (resourceCreateQueue.try_pop(create_resource))
        {
            Spawn_Resource(create_resource.Resource_id, create_resource.Location, create_resource.amount, create_resource.resourcetype);
        }
    }
    s_event create_event;
    while (!EventCreateQueue.empty())
    {
        if (EventCreateQueue.try_pop(create_event))
        {
            Spawn_Event(create_event.e_id, create_event.Location);
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
    if (GameEvent[e_id] != nullptr)
    {
        GameEvent[e_id]->Destroy();
    }
    GameEvent[e_id] = nullptr;
}

void AResourceManager::Set_Resource_Queue(int Resource_id, FVector Location, int amount, char resourcetype)
{
    s_resource save_resource;
    save_resource.amount = amount;
    save_resource.Resource_id = Resource_id;
    save_resource.Location = Location;
    save_resource.resourcetype = resourcetype;

    resourceCreateQueue.push(save_resource);
}

void AResourceManager::Set_Event_Queue(int e_id, FVector Location)
{
    s_event s_insert;
    s_insert.e_id = e_id;
    s_insert.Location = Location;
    EventCreateQueue.push(s_insert);
}

