#include "Instance_Sand.h"

AInstance_Sand::AInstance_Sand()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	SetRootComponent(InstancedStaticMeshComponent);
	InstancedStaticMeshComponent->SetMobility(EComponentMobility::Static);
	InstancedStaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponent->SetGenerateOverlapEvents(false);

}

void AInstance_Sand::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInstance_Sand::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

