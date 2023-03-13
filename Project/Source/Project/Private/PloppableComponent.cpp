#include "PloppableComponent.h"

// Sets default values for this component's properties
UPloppableComponent::UPloppableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPloppableComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnActorBeginOverlap.AddDynamic(this, &UPloppableComponent::OnOverlapEvent);
	GetOwner()->OnActorEndOverlap.AddDynamic(this, &UPloppableComponent::OnOverlapEvent);

}


// Called every frame
void UPloppableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPloppableComponent::OnOverlapEvent(AActor* OverlappedActor, AActor* OtherActor)
{
	UpdateState();
}

void UPloppableComponent::UpdateState()
{
	TArray<AActor*> CollectedActors;
	GetOwner()->GetOverlappingActors(CollectedActors);
	
	IsPlacementValid = (CollectedActors.Num() == 0);

	for (auto& element : GetOwner()->GetComponentsByClass(UActorComponent::StaticClass()))
	{
		UMeshComponent* mesh = Cast<UStaticMeshComponent>(element);
		if (IsPlacementValid == true)
		{
			mesh->SetMaterial(0, PloppableMaterial);
		}
		else {
			mesh->SetMaterial(0, InvalidPloppableMaterial);
		}
	}
}

