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
		bool SprinklerUI = false;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int workcitizen = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int joblessCitizen = 0;

	/// <summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ArmyUI = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TradeUI = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Trade_access = false;


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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* SelectedSprinkler;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int SelectedSprinklerID = -1;

	UFUNCTION(BlueprintCallable)
	void UIClick(bool isplus);

	UFUNCTION(BlueprintCallable)
		void UIClick_army(bool isplus, int armyType);

	UFUNCTION(BlueprintCallable)
	void SendMinimapPacket(float x, float y);

	void select_event(sc_packet_eventselect* );

	UFUNCTION(BlueprintCallable)
	void do_event(int o_select);

	UFUNCTION(BlueprintCallable)
	void Army_Return();

	UFUNCTION(BlueprintCallable)
	void Army_Disband();

	UFUNCTION(BlueprintCallable)
	void Trade_Request(int player_num);

	UFUNCTION(BlueprintCallable)
	void Trade_Access_send(int access);

	int Trade_Request_player;
	int Trade_player;

	void Trade_DisAccess_send(int access, int player_num);

	void Trade_Request_UI(int player_num);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int my_id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int diplomacy_player_id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Trade_Text{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Trade_UI_visible;

	UFUNCTION(BlueprintCallable)
		void diplomacy_player_click(int a);

	UFUNCTION(BlueprintCallable)
		void send_trade_resource_amount(int resource_num, int amount);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int trade_my_oil{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int trade_my_water{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int trade_my_iron{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int trade_my_food{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int trade_my_wood{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int trade_other_oil{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int trade_other_water{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int trade_other_iron{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int trade_other_food{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int trade_other_wood{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool my_trade_deal = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool other_trade_deal = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool my_trade_success = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool other_trade_success = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool War_player0{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool War_player1{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool War_player2{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool War_player3{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool War_player4{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<uint8> War_players;

	void set_war_player(int p_num, int is_war);

	UFUNCTION(BlueprintCallable)
		void send_trade_deal();

	UFUNCTION(BlueprintCallable)
		void send_trade_success();

	UFUNCTION(BlueprintCallable)
		void trade_complete();

	//전쟁
	UFUNCTION(BlueprintCallable)
		void War_Player(int player_num);


	//스프링클러
	UFUNCTION(BlueprintCallable)
		void ChangeSprinklerStatus(bool status, int obj_id);
	UFUNCTION(BlueprintCallable)
		bool GetSprinklerStatus(int obj_id);

	UFUNCTION(BlueprintCallable)
		int GetObjectID();
	UFUNCTION(BlueprintCallable)
		AActor* GetObjectActor();

	void trade_change_resource(int resource_num, int amount);
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


	UFUNCTION(BlueprintCallable)
	void SelectBuildingHouse();
	UFUNCTION(BlueprintCallable)
	void SelectBuildingOilDrill();
	UFUNCTION(BlueprintCallable)
	void SelectBuildingWaterDrill();
	UFUNCTION(BlueprintCallable)
	void SelectBuildingSawMill();
	UFUNCTION(BlueprintCallable)
	void SelectBuildingSteelMill();
	UFUNCTION(BlueprintCallable)
	void SelectBuildingLaboratory();
	UFUNCTION(BlueprintCallable)
	void SelectBuildingSprinkler();
	UFUNCTION(BlueprintCallable)
	void SelectBuildingArmyCamp();
	UFUNCTION(BlueprintCallable)
	void SelectBuildingGuardPost();
	UFUNCTION(BlueprintCallable)
	void SelectBuildingGreenHouse();
	

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
