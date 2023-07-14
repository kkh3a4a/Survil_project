// Fill out your copyright notice in the Description page of Project Settings.


#include "CitizenManager.h"
#include "Kismet/GameplayStatics.h"
#include "Main.h"
#include "NetworkingThread.h"


// Sets default values
ACitizenManager::ACitizenManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    UWorld* worldref = GetWorld();
    if (worldref == nullptr) {
        return;
    }

    AActor* actor = UGameplayStatics::GetActorOfClass(worldref, AMain::StaticClass());
    if (actor == nullptr) {
        return;
    }
    actor->GetWorld();
    Main = Cast<AMain>(actor);
    Network = nullptr;
}

// Called when the game starts or when spawned
void ACitizenManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACitizenManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (Network == nullptr)
    {
        if(Main == nullptr)
        {
            UE_LOG(LogTemp, Log, TEXT("Manager connect"));
        }
        Network = reinterpret_cast<FSocketThread*>(Main->Network);
        Network->_CitizenManager = this;
        UE_LOG(LogTemp, Log, TEXT("Manager connect"));
    }
}

void ACitizenManager::Spawn_Citizen(int citizen_id, FVector Location)
{
    FActorSpawnParameters SpawnInfo;
    UWorld* uworld = GetWorld();
    if (uworld == nullptr)
        return;
    if(citizen[citizen_id] == nullptr)
    {
        if (Network->my_id == (citizen_id / 200))
            citizen[citizen_id] = uworld->SpawnActor<ACitizen>(MyCitizen_MODEL, Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
        else  if (Network->my_id != (citizen_id / 200))
            citizen[citizen_id] = uworld->SpawnActor<ACitizen>(EnemyCitizen_MODEL, Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
    }
    else
    {
        citizen[citizen_id]->SetActorHiddenInGame(false);
        citizen[citizen_id]->SetActorLocation(Location);
    }

    ACitizen* citi = reinterpret_cast<ACitizen*>(citizen[citizen_id]);
    citi->_id = citizen_id;
}

void ACitizenManager::Set_Citizen_Location(int citizen_id, FVector Location, FRotator Rotate, char citizenstate)
{
    if(citizen[citizen_id] != nullptr)
    {
        if (citizen[citizen_id]->GetWorld() && citizen[citizen_id]->IsValidLowLevel())
        {
            citizen[citizen_id]->SetActorLocation(Location);
            citizen[citizen_id]->SetActorRotation(Rotate);
            ACitizen* citi = reinterpret_cast<ACitizen*>(citizen[citizen_id]);
            citi->state = citizenstate;
        }
    }
}

void ACitizenManager::Remove_Citizen(int citizen_id)
{
    if (citizen[citizen_id] != nullptr)
    {
        citizen[citizen_id]->Destroy();
        citizen[citizen_id] = nullptr;
    }
}

void ACitizenManager::Spawn_Army(void* buf)
{
    sc_packet_armytraining* packet = reinterpret_cast<sc_packet_armytraining*>(buf);
    citizen[packet->c_id1 - CITIZENSTART]->SetActorHiddenInGame(true);
    citizen[packet->c_id2 - CITIZENSTART]->SetActorHiddenInGame(true);
    citizen[packet->c_id3 - CITIZENSTART]->SetActorHiddenInGame(true);
    citizen[packet->c_id4 - CITIZENSTART]->SetActorHiddenInGame(true);
    citizen[packet->c_id5 - CITIZENSTART]->SetActorHiddenInGame(true);
    FVector Location{ packet->x,packet->y,packet->z };
    FActorSpawnParameters SpawnInfo;
    UWorld* uworld = GetWorld();
    if(packet->_army_type == 0)
    {
        army[packet->army_id - ARMYSTART] = uworld->SpawnActor<AActor>(ARMY_MODEL, Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
        army[packet->army_id - ARMYSTART]->Tags.Add(TEXT("Army"));
        army[packet->army_id - ARMYSTART]->Tags.Add(*FString::FromInt(0));
        
    }
    else if (packet->_army_type == 1)
    {
        army[packet->army_id - ARMYSTART] = uworld->SpawnActor<AActor>(ARMY_MODEL1, Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
        army[packet->army_id - ARMYSTART]->Tags.Add(TEXT("Army"));
        army[packet->army_id - ARMYSTART]->Tags.Add(*FString::FromInt(packet->_army_type));

    }
    else if (packet->_army_type == 2)
    {
        army[packet->army_id - ARMYSTART] = uworld->SpawnActor<AActor>(ARMY_MODEL2, Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
        army[packet->army_id - ARMYSTART]->Tags.Add(TEXT("Army"));
        army[packet->army_id - ARMYSTART]->Tags.Add(*FString::FromInt(packet->_army_type));

    }
    
    army[packet->army_id - ARMYSTART]->Tags.Add(*FString::FromInt(packet->army_id - ARMYSTART));
    army[packet->army_id - ARMYSTART]->Tags.Add(*FString::FromInt((packet->army_id - ARMYSTART) / (ARMYMAX / 5)));
    AArmy* army_class = reinterpret_cast<AArmy*>(army[packet->army_id - ARMYSTART]);
    army_class->Army_HP = packet->hp;
}

void ACitizenManager::Set_Army_Location(int a_id, FVector Location, FRotator Rotate, char a_state)
{
    if (army[a_id] != nullptr)
    {
        if (army[a_id]->GetWorld() && army[a_id]->IsValidLowLevel())
        {
            army[a_id]->SetActorLocation(Location);
            army[a_id]->SetActorRotation(Rotate);
            AArmy* a = reinterpret_cast<AArmy*>(army[a_id]);
            a->state = a_state;
        }
    }

}

void ACitizenManager::Set_Army_Disband(int a_id)
{
    if (army[a_id] != nullptr)
    {
        army[a_id]->Destroy();
        army[a_id] = nullptr;
    }

}
