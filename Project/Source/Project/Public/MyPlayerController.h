// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include <chrono>
#include "global.h"
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
	AActor* hitActor;

	AActor* server;

	std::chrono::steady_clock::time_point mouse_start_t;
	std::chrono::steady_clock::time_point mouse_end_t;

	bool temped;

protected:
	bool bRightClickMouse;

	bool bLeftClickMouse;

	void InputRightMoustButtonPressed();

	void InputRightMoustButtonReleased();

	void InputLeftMoustButtonPressed();

	void InputLeftMoustButtonReleased();

	void InputUpPressed();

	void InputDownPressed();

	void InputLeftPressed();

	void InputRightPressed();

	void InputUpReleased();

	void InputDownReleased();

	void InputLeftReleased();

	void InputRightReleased();

	//void SetNewDestination(const FVector DestLocation);

	void MoveToMouseCursor();

	void MoveToActor();

	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;

// grid and building ±¸ºÐ
//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	bool PlacementModeEnabled;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	AActor* PlaceableActor;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TSubclassOf<AActor> PlaceableActorType;
//
//	void SetPlacementModeEnabled(bool IsEnabled);
//
//	void UpdatePlacement();
//
//	void SpawnBuilding();
};
