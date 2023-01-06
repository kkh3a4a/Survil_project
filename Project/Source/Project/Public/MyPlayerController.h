// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DestLocation;

protected:
	bool bClickMouse;

	void InputLeftMoustButtonPressed();

	void InputLeftMoustButtonReleased();

	//void SetNewDestination(const FVector DestLocation);

	void MoveToMouseCursor();

	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;
};
