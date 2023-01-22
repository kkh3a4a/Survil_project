#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Instance_Sand.generated.h"

UCLASS()
class PROJECT_API AInstance_Sand final : public AActor
{
	GENERATED_BODY()
	
public:	
	AInstance_Sand();


protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaSeconds) override;

#pragma region Components

protected:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

#pragma endregion
};
