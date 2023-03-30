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
    if(Network->my_id == (citizen_id / 200))
         citizen[citizen_id] = GetWorld()->SpawnActor<ACitizen>(MyCitizen_MODEL, Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
    else  if (Network->my_id != (citizen_id / 200))
         citizen[citizen_id] = GetWorld()->SpawnActor<ACitizen>(EnemyCitizen_MODEL, Location, FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);

}

void ACitizenManager::Set_Citizen_Location(int citizen_id, FVector Location, FRotator Rotate)
{
    if (citizen[citizen_id]->GetWorld() && citizen[citizen_id]->IsValidLowLevel())
    {
        citizen[citizen_id]->SetActorLocation(Location);
        citizen[citizen_id]->SetActorRotation(Rotate);
    }
}


