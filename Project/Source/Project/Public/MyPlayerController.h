// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include <chrono>
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Temperature.h"
#include "BuildManager.h"
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
	int ClickObjectType = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ResourceCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool BuildingUI = false;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int workcitizen = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int joblessCitizen = 0;

	/// <summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ArmyUI = false;





	/// ///////////////////////////////
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool GameEventUI = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Summary_text {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText First_text {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Second_text {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Third_text {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int e_select = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int e_type = 0;

	int e_id = 0;
	/// //////////////////////////////////////////
	AActor* ResourceActor;

	UFUNCTION(BlueprintCallable)
	void UIClick(bool isplus);

	UFUNCTION(BlueprintCallable)
	void SendMinimapPacket(float x, float y);

	void select_event(sc_packet_eventselect* );

	UFUNCTION(BlueprintCallable)
	void do_event(int o_select);

	UFUNCTION(BlueprintCallable)
	void Army_Return();

	UFUNCTION(BlueprintCallable)
	void Army_Disband();



protected:
	long long int mouse_cnt = 0;

	bool bRightClickMouse;
	bool bLeftClickMouse;

	void InputLeftMoustButtonPressed();
	void InputRightMoustButtonPressed();
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

	UFUNCTION(BlueprintCallable)
	void BuildMode();
	void SelectBuildingHouse();
	void SelectBuildingHouse2();
	void SelectBuildingHouse3();
	void SelectBuildingHunterHouse();
	void SelectBuildingARMYCAMP();
	void OnBuildMode();
	void MouseScrollUp();
	void MouseScrollDown();

	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	

	//////////////////////
	class AMain* Main_Class;
	class AResourceManager* ResourceManager;
	class ABuildManager* BuildManager;
	class FSocketThread* Network;
	char Key_w, Key_a, Key_s, Key_d;

	void SendMovePacket();
	
};
