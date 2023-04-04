// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include <chrono>
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

	bool temped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ResourceUI = false;

	int ObjectType = 0;	//현재 클릭된 Object종류에따라 START위치를 더해줘서 id만들어주고 보내야함
	int ObjectId = -1;

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
	int workcitizen = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int joblessCitizen = 0;


	AActor* ResourceActor;

	UFUNCTION(BlueprintCallable)
	void UIClick(bool isplus);

protected:
	long long int mouse_cnt = 0;

	bool bRightClickMouse;
	bool bLeftClickMouse;

	void InputLeftMoustButtonPressed();
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
	void SelectBuilding1();
	void SelectBuilding2();
	void SelectBuilding3();
	void SelectBuilding4();

	void OnBuildMode();
	void MouseScrollUp();
	void MouseScrollDown();

	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	

	//////////////////////
	class AMain* Main_Class;
	class AResourceManager* ResourceManager;
	class FSocketThread* Network;
	char Key_w, Key_a, Key_s, Key_d;

	void SendMovePacket();
};
