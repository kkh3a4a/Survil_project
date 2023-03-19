// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include <chrono>
#include "global.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Temperature.h"
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

	std::chrono::steady_clock::time_point mouse_start_t;
	std::chrono::steady_clock::time_point mouse_end_t;

	bool temped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ResourceUI = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ResourceType = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ResourceCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CitizenAdd = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CitizenSub = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CitizenRelease = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CitizenCount=0;


	AActor* ResourceActor;

protected:
	long long int mouse_cnt = 0;

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
	void MoveToMouseCursor();
	void MoveToActor();
	void VisibilityTemperature();
	void BuildMode();
	void Build();
	void MouseScrollUp();
	void MouseScrollDown();

	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;


};
