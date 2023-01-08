// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Server_testing.h"
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
	AServer_testing* server_testing;
protected:
	bool bRightClickMouse;

	bool bLeftClickMouse;

	void InputRightMoustButtonPressed();

	void InputRightMoustButtonReleased();

	void InputLeftMoustButtonPressed();

	void InputLeftMoustButtonReleased();

	//void SetNewDestination(const FVector DestLocation);

	void MoveToMouseCursor();

	void MoveToActor();

	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;
};
