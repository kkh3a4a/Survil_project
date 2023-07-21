// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Main.h"
#include "NetworkingThread.h"
#include "ResourceManager.h"
//#include "Army.h"
//#include "Blueprint/AIBlueprintHelperLibrary.h"

AMyPlayerController::AMyPlayerController()
{

    UWorld* worldref = GetWorld();
    if (worldref == nullptr){
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("%s"), *worldref->GetName());
    
    AActor* actor = UGameplayStatics::GetActorOfClass(worldref, AMain::StaticClass());
    if (actor == nullptr){
        return;
    }
    actor->GetWorld();
    Main_Class = Cast<AMain>(actor);
    if (Main_Class == nullptr){
        return; 
    }
    AActor* resourceManagerActor = UGameplayStatics::GetActorOfClass(worldref, AResourceManager::StaticClass());
    if (resourceManagerActor == nullptr) {
        return;
    }
    ResourceManager = Cast < AResourceManager>(resourceManagerActor);
    if (ResourceManager == nullptr) {
        return;
    }

    AActor* BuildManagerActor = UGameplayStatics::GetActorOfClass(worldref, ABuildManager::StaticClass());
    if (BuildManagerActor == nullptr) {
        return;
    }
    BuildManager = Cast <ABuildManager>(BuildManagerActor);
    if (BuildManager == nullptr) {
        return;
    }

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableTouchEvents = true;
    bEnableMouseOverEvents = true;
    Network = nullptr;
    Key_w = false;
    Key_a = false;
    Key_s = false;
    Key_d = false;
    War_players.Init(false, 5);
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMyPlayerController::InputLeftMoustButtonPressed);
    InputComponent->BindAction("RightClick", IE_Pressed, this, &AMyPlayerController::InputRightMoustButtonPressed);

    InputComponent->BindAction("Up", IE_Pressed, this, &AMyPlayerController::InputUpPressed);
    InputComponent->BindAction("Up", IE_Released, this, &AMyPlayerController::InputUpReleased);

    InputComponent->BindAction("Down", IE_Pressed, this, &AMyPlayerController::InputDownPressed);
    InputComponent->BindAction("Down", IE_Released, this, &AMyPlayerController::InputDownReleased);

    InputComponent->BindAction("Left", IE_Pressed, this, &AMyPlayerController::InputLeftPressed);
    InputComponent->BindAction("Left", IE_Released, this, &AMyPlayerController::InputLeftReleased);

    InputComponent->BindAction("Right", IE_Pressed, this, &AMyPlayerController::InputRightPressed);
    InputComponent->BindAction("Right", IE_Released, this, &AMyPlayerController::InputRightReleased);

	InputComponent->BindAction("Thermal", IE_Pressed, this, &AMyPlayerController::VisibilityTemperature);
    InputComponent->BindAction("army_select_key1", IE_Pressed, this, &AMyPlayerController::InputQ);
    InputComponent->BindAction("army_select_key2", IE_Pressed, this, &AMyPlayerController::InputE);
    /*InputComponent->BindAction("Build", IE_Pressed, this, &AMyPlayerController::BuildMode);
    InputComponent->BindAction("1", IE_Pressed, this, &AMyPlayerController::SelectBuildingHouse);
    InputComponent->BindAction("2", IE_Pressed, this, &AMyPlayerController::SelectBuildingWaterDrill);
    InputComponent->BindAction("3", IE_Pressed, this, &AMyPlayerController::SelectBuildingLaboratory);
    InputComponent->BindAction("4", IE_Pressed, this, &AMyPlayerController::SelectBuildingGuardPost);
    InputComponent->BindAction("5", IE_Pressed, this, &AMyPlayerController::SelectBuildingArmyCamp);*/

    InputComponent->BindAction("MouseScrollUp", IE_Pressed, this, &AMyPlayerController::MouseScrollUp);
    InputComponent->BindAction("MouseScrollDown", IE_Pressed, this, &AMyPlayerController::MouseScrollDown);
}

void AMyPlayerController::InputUpPressed()
{
    Key_w = true;
}

void AMyPlayerController::InputDownPressed()
{
    Key_s = true;
}

void AMyPlayerController::InputLeftPressed()
{
    Key_a = true;
}

void AMyPlayerController::InputRightPressed()
{
    Key_d = true;
}



void AMyPlayerController::InputUpReleased()
{
    Key_w = false;
}

void AMyPlayerController::InputDownReleased()
{
    Key_s = false;
}

void AMyPlayerController::InputLeftReleased()
{
    Key_a = false;
}

void AMyPlayerController::InputRightReleased()
{
    Key_d = false;
}



void AMyPlayerController::UIClick(bool isplus)
{
    cs_packet_citizenplacement packet;
    packet.size = sizeof(cs_packet_citizenplacement);
    packet.type = CS_PACKET_CITIZENPLACEMENT;
    packet.objectid = ObjectId + ObjectType;
    packet.isplus = isplus;
    UE_LOG(LogTemp, Log, TEXT("UIClick"));
    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
    UE_LOG(LogTemp, Log, TEXT("UIClick sent"));
}

void AMyPlayerController::UIClick_army(bool isplus, int armyType)
{
    cs_packet_citizenplacement packet;
    packet.size = sizeof(cs_packet_citizenplacement);
    packet.type = CS_PACKET_CITIZENPLACEMENT;
    packet.objectid = ObjectId + ObjectType;
    packet.isplus = isplus;
    packet.army_type = armyType;
    UE_LOG(LogTemp, Log, TEXT("UIClick_army"));
    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}

void AMyPlayerController::set_war_player(int p_num, int is_war)
{
    War_players[p_num] = is_war;
}

void AMyPlayerController::send_trade_deal()
{
    if (my_trade_deal)
        my_trade_deal = false;
    else
        my_trade_deal = 1;

    cs_packet_tradedeal packet;
    packet.deal_boolean = my_trade_deal;
    packet.size = sizeof(packet);
    packet.type = CS_PACKET_TRADEDEAL;

    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}

void AMyPlayerController::send_trade_success()
{
    if (my_trade_success)
        my_trade_success = false;
    else
        my_trade_success = 1;

    cs_packet_tradesuccess packet;
    packet.size = sizeof(packet);
    packet.type = CS_PACKET_TRADESUCCESS;
    packet.success_boolean = my_trade_success;

    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);

}

void AMyPlayerController::trade_complete()
{
    trade_my_oil = 0;
    trade_my_water = 0;
    trade_my_iron = 0;
    trade_my_food = 0;
    trade_my_wood = 0;
    trade_other_oil = 0;
    trade_other_water = 0;
    trade_other_iron = 0;
    trade_other_food = 0;
    trade_other_wood = 0;
    my_trade_deal = false;
    my_trade_success = false;
    other_trade_deal = false;
    other_trade_success = false;

}

void AMyPlayerController::War_Player(int player_num)
{
    cs_packet_declaration_war packet;
    packet.size = sizeof(cs_packet_declaration_war);
    packet.type = CS_PACKET_DECLARATION_WAR;
    packet.player_num = player_num;

    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}

void AMyPlayerController::ChangeSprinklerStatus(bool status, int obj_id)
{
    bool changed = false;
    if (status) {
        if (Main_Class->BuildManager->BuiltBuildings[obj_id]->Tags[4] == TEXT("OFF")) {
            Main_Class->BuildManager->BuiltBuildings[obj_id]->Tags.Remove(FName("OFF"));
            Main_Class->BuildManager->BuiltBuildings[obj_id]->Tags.Add(FName("ON"));
            changed = true;
        }
    }
    else {
        if (Main_Class->BuildManager->BuiltBuildings[obj_id]->Tags[4] == TEXT("ON")) {
            Main_Class->BuildManager->BuiltBuildings[obj_id]->Tags.Remove(FName("ON"));
            Main_Class->BuildManager->BuiltBuildings[obj_id]->Tags.Add(FName("OFF"));
            changed = true;
        }
    }
    if (changed) {
        cs_packet_sprinkler_status packet;
        packet.status = status;
        packet.sprinkler_id = obj_id;
        WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
        WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
    }
}

bool AMyPlayerController::GetSprinklerStatus(int obj_id)
{
    if (Main_Class->BuildManager->BuiltBuildings[obj_id]->Tags[4] == TEXT("ON"))
        return true;
    else
        return false;
}

int AMyPlayerController::GetObjectID()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);
    if (Hit.bBlockingHit)
    {
        AActor* Actor = Hit.GetActor();
        UE_LOG(LogTemp, Log, TEXT("%s"), *hitActor->GetName());
        int ObjectID = FCString::Atoi(*hitActor->Tags[2].ToString());
        return ObjectID;
    }
    else 
    {
        return -1;
    }
}

AActor* AMyPlayerController::GetObjectActor()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);
    if (Hit.bBlockingHit)
    {
        AActor* Actor = Hit.GetActor();
        return Actor;
    }
    else 
    {
        return nullptr;
    }
}

void AMyPlayerController::trade_change_resource(int resource_num, int amount)
{
    switch (resource_num)
    {
    case 0:
    {
        trade_my_oil = amount;
        break;
    }
    case 1:
    {
        trade_my_water = amount;
        break;
    }
    case 2:
    {
        trade_my_iron = amount;
        break;
    }
    case 3:
    {
        trade_my_food = amount;
        break;
    }
    case 4:
    {
        trade_my_wood = amount;
        break;
    }
    case 5:
    {
        trade_other_oil = amount;
        break;
    }
    case 6:
    {
        trade_other_water = amount;
        break;
    }
    case 7:
    {
        trade_other_iron = amount;
        break;
    }
    case 8:
    {
        trade_other_food = amount;
        break;
    }
    case 9:
    {
        trade_other_wood = amount;
        break;
    }
    default:
        break;
    }
    other_trade_deal = false;
}

void AMyPlayerController::InputLeftMoustButtonPressed()
{
    if (BuildManager->BuildMode) {
        BuildManager->SendBuildPacket();
    }
    else {
        MoveToMouseCursor();
    }
}

void AMyPlayerController::InputRightMoustButtonPressed()
{
    if (BuildManager->BuildMode) {
    }
    else {
        MoveToActor();
    }
}

void AMyPlayerController::MoveToMouseCursor()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        hitActor = Hit.GetActor();
        UE_LOG(LogTemp, Log, TEXT( "%s"), * hitActor->GetName());
        if (hitActor->ActorHasTag("Citizen"))
        {
            UE_LOG(LogTemp, Log, TEXT("Citizen"));
            if (wcscmp(*hitActor->Tags[1].ToString(), L"0") == 0)
            {

            }
            else
            {
                hitActor = NULL;
            }
            ResourceUI = false;
            BuildingUI = false;
            SprinklerUI = false;
            ArmyUI = false;
        }
        else if (hitActor->ActorHasTag("Resource"))
        {
            ResourceActor = hitActor;
            
            ObjectType = RESOURCESTART;
            ObjectId = FCString::Atoi(*hitActor->Tags[1].ToString());
            ClickObjectType = ResourceManager->resource_type[ObjectId];
            ResourceCount = ResourceManager->resource_amount[ObjectId];
            workcitizen = ResourceManager->workCitizens[ObjectId];
            joblessCitizen = Network->playerjobless;


            ResourceUI = true;
            BuildingUI = false;
            SprinklerUI = false;
            ArmyUI = false;
        }
        else if (hitActor->ActorHasTag("Building"))
        {
            UE_LOG(LogTemp, Log, TEXT("type : %d"), FCString::Atoi(*hitActor->Tags[1].ToString()));
            if (FCString::Atoi(*hitActor->Tags[3].ToString()) != my_id)
            {
                ObjectId = -1;
                ObjectType = 0;
                ResourceUI = false;
                BuildingUI = false;
                SprinklerUI = false;
                ArmyUI = false;
                hitActor = NULL;
                return;
            }
            ObjectType = BUILDINGSTART;

            ObjectId = FCString::Atoi(*hitActor->Tags[2].ToString());
            ClickObjectType = FCString::Atoi(*hitActor->Tags[1].ToString());
            workcitizen = BuildManager->buildingWorkCount[ObjectId];
            joblessCitizen = Network->playerjobless;

            ResourceUI = false;
            ArmyUI = false;
            if (FCString::Atoi(*hitActor->Tags[1].ToString()) == 8) {
                SprinklerUI = true;
                UE_LOG(LogTemp, Log, TEXT("sprinkler\n"));
                BuildingUI = false;
                SelectedSprinkler = hitActor;
                SelectedSprinklerID = ObjectId;
            }
            else {
                BuildingUI = true;
                SprinklerUI = false;
            }
        }
        else if (hitActor->ActorHasTag("Army"))
        {
            UE_LOG(LogTemp, Log, TEXT("Army"));
            if (FCString::Atoi(*hitActor->Tags[3].ToString()) != my_id)
            {
                ObjectId = -1;
                ObjectType = 0;
                ResourceUI = false;
                BuildingUI = false;
                SprinklerUI = false;
                ArmyUI = false;
                hitActor = NULL;
                return;
            }
            ObjectType = ARMYSTART;
            ObjectId = FCString::Atoi(*hitActor->Tags[2].ToString());
            
            ClickObjectType = FCString::Atoi(*hitActor->Tags[1].ToString());
            UE_LOG(LogTemp, Log, TEXT("type : %d"), ClickObjectType);
            ResourceUI = false;
            BuildingUI = false;
            ArmyUI = true;
        }
        else
        {
            ObjectId = -1;
            ObjectType = 0;
            ResourceUI = false;
            BuildingUI = false;
            SprinklerUI = false;
            ArmyUI = false;
            hitActor = NULL;
        }
    }
}

void AMyPlayerController::MoveToActor()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);
    if (hitActor != NULL)
    {
        if (hitActor->ActorHasTag("Army"))
        {
            cs_packet_armymove packet;
         
            packet.type = CS_PACKET_ARMYMOVE;
            packet.size = sizeof(cs_packet_armymove);

            packet.a_id = ObjectId + ObjectType;
            packet.x = Hit.Location.X;
            packet.y = Hit.Location.Y;

            WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
            WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
        }
    }
    UE_LOG(LogTemp, Log, TEXT("type : %lf"), Hit.Location.X)
}

void AMyPlayerController::VisibilityTemperature()
{
    ATemperature* TemperatureClass = Main_Class->Temperature;


    bool Hidden = TemperatureClass->GetIsHidden();
    if (Hidden) {                                   //켜기
        UE_LOG(LogTemp, Log, TEXT("Thermal On"));

        TemperatureClass->Hide(false);
        Main_Class->SunManager->SetActorHiddenInGame(true);
        TemperatureClass->SetActorLocation(FVector(Main_Class->GetActorLocation().X, Main_Class->GetActorLocation().Y, 0.f));

        if (BuildManager->BuildMode) {      //build모드 켜져있으면 끄기
            BuildMode();
        }
	}
    else{                                           //끄기
        UE_LOG(LogTemp, Log, TEXT("Thermal Off"));

        TemperatureClass->Hide(true);
        Main_Class->SunManager->SetActorHiddenInGame(false);
	}
    
}

void AMyPlayerController::BuildMode()
{
    if (BuildManager->BuildMode) {
        BuildManager->BuildMode = false;
        UE_LOG(LogTemp, Log, TEXT("BuildMode Off"));
    }
    else {
        BuildManager->BuildMode = true;
        UE_LOG(LogTemp, Log, TEXT("BuildMode On"));

        ATemperature* TemperatureClass = Main_Class->Temperature;   //온도 켜져있으면 끄기
        if (!TemperatureClass->GetIsHidden()) {
            VisibilityTemperature();
        }
    }
    BuildManager->DecalVisibility();
}

void AMyPlayerController::OnBuildMode()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);
    if (Hit.bBlockingHit)
    {
        BuildManager->UpdateDecalPosition(Hit.ImpactPoint, Main_Class->Player_x, Main_Class->Player_y);
    }
}

void AMyPlayerController::SelectBuildingHouse()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 1;

        BuildManager->DecalActor->SetActorScale3D(FVector(10.f, 1.6f, 1.6f));
    }
}

void AMyPlayerController::SelectBuildingOilDrill()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 2;

        BuildManager->DecalActor->SetActorScale3D(FVector(10.f, 1.6f, 1.6f));
    }
}

void AMyPlayerController::SelectBuildingWaterDrill()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 3;

        BuildManager->DecalActor->SetActorScale3D(FVector(10.f, 1.6f, 1.6f));
    }
}

void AMyPlayerController::SelectBuildingSawMill()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 4;

        BuildManager->DecalActor->SetActorScale3D(FVector(10.f, 1.6f, 1.6f));
    }
}

void AMyPlayerController::SelectBuildingSteelMill()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 5;

        BuildManager->DecalActor->SetActorScale3D(FVector(10.f, 1.6f, 1.6f));
    }
}

void AMyPlayerController::SelectBuildingLaboratory()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 6;

        BuildManager->DecalActor->SetActorScale3D(FVector(10.f, 1.6f, 1.6f));

    }
}

//void AMyPlayerController::SelectBuildingNursery(bool status, int obj_id)
//{
//}

void AMyPlayerController::InputQ()
{
    cs_packet_armyselect packet;
    packet.size = sizeof(packet);
    packet.type = CS_PACKET_ARMYSELECT;
    packet.select_type = 0;
    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}

void AMyPlayerController::InputE()
{
    cs_packet_armyselect packet;
    packet.size = sizeof(packet);
    packet.type = CS_PACKET_ARMYSELECT;
    packet.select_type = 1;
    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}

void AMyPlayerController::SelectBuildingSprinkler()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 8;

        BuildManager->DecalActor->SetActorScale3D(FVector(10.f, 0.4f, 0.4f));
    }
}

void AMyPlayerController::SelectBuildingArmyCamp()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 21;

        BuildManager->DecalActor->SetActorScale3D(FVector(10.f, 1.6f, 1.6f));
    }
}

void AMyPlayerController::SelectBuildingGuardPost()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 11;       

        BuildManager->DecalActor->SetActorScale3D(FVector(10.f, 1.6f, 1.6f));
    }
}

void AMyPlayerController::SelectBuildingGreenHouse()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 13;

        BuildManager->DecalActor->SetActorScale3D(FVector(10.f, 1.6f, 1.6f));
    }
}


void AMyPlayerController::MouseScrollUp()
{
   if (Main_Class->MyCamera->GetActorLocation().Z <= 1000)
        return;
    Main_Class->MyCamera->SetActorLocation(Main_Class->MyCamera->GetActorLocation() - FVector(0, 100, 500));
    Main_Class->MyCamera->SetActorRotation(FRotator(Main_Class->MyCamera->GetActorRotation().Pitch + 2.45, Main_Class->MyCamera->GetActorRotation().Yaw, Main_Class->MyCamera->GetActorRotation().Roll));
}

void AMyPlayerController::MouseScrollDown()
{
    if (Main_Class->MyCamera->GetActorLocation().Z >= 6500)
        return;
    Main_Class->MyCamera->SetActorLocation(Main_Class->MyCamera->GetActorLocation() + FVector(0, 100, 500));
    Main_Class->MyCamera->SetActorRotation(FRotator(Main_Class->MyCamera->GetActorRotation().Pitch - 2.45, Main_Class->MyCamera->GetActorRotation().Yaw, Main_Class->MyCamera->GetActorRotation().Roll));
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if(Network == nullptr)
    {
        Network = reinterpret_cast<FSocketThread*>(Main_Class->Network);
        Network->_MyController = this;
    }

    mouse_cnt++;
    //UE_LOG(LogTemp, Log, TEXT("%s : %lf, %lf"),  *(ServerClass->MouseInput.name), ServerClass->MouseInput.location.x, ServerClass->MouseInput.location.y);
    //오른쪽 클릭 작업
    if(hitActor != NULL)
    {
       if (ResourceUI == true)
        {
            ResourceCount = ResourceManager->resource_amount[ObjectId];
            workcitizen = ResourceManager->workCitizens[ObjectId];
            joblessCitizen = Network->playerjobless;
        }
        else if (BuildingUI == true)
        {
            joblessCitizen = Network->playerjobless;
            workcitizen = BuildManager->buildingWorkCount[ObjectId];
        }
        else if (ArmyUI == true)
       {
           joblessCitizen = Network->playerjobless;
       }
        else
        {
            ClickObjectType = 0;
            ResourceCount = 0;
            workcitizen = 0;
            joblessCitizen = 0;
        }
    }
    
    if (BuildManager->BuildMode) {
        OnBuildMode();
    }

    //key check
    if (Key_w || Key_a || Key_s || Key_d)
        SendMovePacket();

}

void AMyPlayerController::SendMovePacket()
{
    //UE_LOG(LogTemp, Log, TEXT("send move packet"));

    cs_packet_move packet;
    packet.w = Key_w;
    packet.a = Key_a;
    packet.s = Key_s;
    packet.d = Key_d;
    packet.size = sizeof(cs_packet_move);
    packet.type = CS_PACKET_MOVE;
    //UE_LOG(LogTemp, Log, TEXT("send Move"));
    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}

void AMyPlayerController::SendMinimapPacket(float x, float y)
{
    cs_packet_minimap packet;
    packet.type = CS_PACKET_MINIMAP;
    packet.size = sizeof(cs_packet_minimap);
   /* x = (((int)x / 10) * 10);
    y = (((int)(y - 720) / 10) * 10);*/
    if (x > 300)
        x = 299;
    if (y > 300)
        y = 299;
  
    x = ((int)(x) * 1000);
    y = ((int)(y) * 1000);
    packet.x = x;
    packet.y = y;
    UE_LOG(LogTemp, Log, TEXT("%f %f"),x,y);
    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}

void AMyPlayerController::select_event(sc_packet_eventselect* packet)
{
    GameEventUI = true;
    Summary_text = FText::FromString(packet->summary);
    First_text = FText::FromString(packet->first);
    Second_text = FText::FromString(packet->second);
    Third_text = FText::FromString(packet->third);
    e_id = packet->e_id;
    e_select = packet->s_option;
    e_type = packet->e_type;

}

void AMyPlayerController::do_event(int o_select)
{
    cs_packet_eventselect packet;
    packet.e_id = e_id;
    packet.s_option = o_select;
 
    packet.size = sizeof(packet);
    packet.type = CS_PACKET_EVENTSELECT;

    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);

    GameEventUI = false;
}

void AMyPlayerController::Army_Return()
{
    cs_packet_armyreturn packet;
    packet.size = sizeof(packet);
    packet.type = CS_PACKET_ARMYRETURN;
    packet.a_id = ObjectId + ObjectType;

    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);

}

void AMyPlayerController::Army_Disband()
{
    cs_packet_armydisband packet;
    packet.a_id = ObjectId + ObjectType;
    packet.size = sizeof(packet);
    packet.type = CS_PACKET_ARMYDISBAND;

    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}

void AMyPlayerController::Trade_Request(int player_num)
{
    cs_packet_traderequest packet;
    packet.request_player = player_num;
    packet.size = sizeof(cs_packet_traderequest);
    packet.type = CS_PACKET_TRADEREQUEST;

    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}

void AMyPlayerController::Trade_Access_send(int access)
{
    cs_packet_tradeagree packet;
    packet.isagree = access;
    packet.size = sizeof(packet);
    packet.type = CS_PACKET_TRADEAGREE;
    packet.request_player = Trade_Request_player;
    if (access == 1)
    {
        Trade_player = Trade_Request_player;
        Trade_UI_visible;
    }

    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
    TradeUI = false;
}

void AMyPlayerController::Trade_DisAccess_send(int access, int player_num)
{
    cs_packet_tradeagree packet;
    packet.isagree = access;
    packet.size = sizeof(packet);
    packet.type = CS_PACKET_TRADEAGREE;
    packet.request_player = player_num;


    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}

void AMyPlayerController::Trade_Request_UI(int player_num)
{
    Trade_Text = FText::FromString(FString::Printf(TEXT("player %d Trade"), player_num));
    Trade_Request_player = player_num;
    TradeUI = true;
}

void AMyPlayerController::diplomacy_player_click(int click_player)
{
    if (click_player == my_id)
        return;

    diplomacy_player_id = click_player;
}

void AMyPlayerController::send_trade_resource_amount(int resource_num, int amount)
{
    switch (resource_num)
    {
    case 0:
    {
        trade_my_oil = amount;
        break;
    }
    case 1:
    {
        trade_my_water = amount;
        break;
    }
    case 2:
    {
        trade_my_iron = amount;
        break;
    }
    case 3:
    {
        trade_my_food = amount;
        break;
    }
    case 4:
    {
        trade_my_wood = amount;
        break;
    }
    case 5:
    {
        trade_other_oil = amount;
        break;
    }
    case 6:
    {
        trade_other_water = amount;
        break;
    }
    case 7:
    {
        trade_other_iron = amount;
        break;
    }
    case 8:
    {
        trade_other_food = amount;
        break;
    }
    case 9:
    {
        trade_other_wood = amount;
        break;
    }
    default:
        break;
    }
    cs_packet_traderesource packet;
    packet.resource_num = resource_num;
    packet.resource_amount = amount;
    packet.size = sizeof(packet);
    packet.type = CS_PACKET_TRADERESOURCE;

    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}
