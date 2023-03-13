// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PloppableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_API UPloppableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPloppableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsPlacementValid;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* PloppableMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* InvalidPloppableMaterial;

	UFUNCTION(Category = "Building")
	void UpdateState();

	UFUNCTION(Category = "Building")
	void OnOverlapEvent(class AActor* OverlappedActor, class AActor* OtherActor);
};
