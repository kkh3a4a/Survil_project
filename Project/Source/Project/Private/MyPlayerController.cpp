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

    InputComponent->BindAction("Build", IE_Pressed, this, &AMyPlayerController::BuildMode);
    InputComponent->BindAction("1", IE_Pressed, this, &AMyPlayerController::SelectBuildingHouse);
    InputComponent->BindAction("2", IE_Pressed, this, &AMyPlayerController::SelectBuildingHouse2);
    InputComponent->BindAction("3", IE_Pressed, this, &AMyPlayerController::SelectBuildingHouse3);
    InputComponent->BindAction("4", IE_Pressed, this, &AMyPlayerController::SelectBuildingHunterHouse);
    InputComponent->BindAction("5", IE_Pressed, this, &AMyPlayerController::SelectBuildingARMYCAMP);

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
        }
        else if (hitActor->ActorHasTag("Building"))
        {
            UE_LOG(LogTemp, Log, TEXT("type : %d"), FCString::Atoi(*hitActor->Tags[1].ToString()));
            ObjectType = BUILDINGSTART;
            ObjectId = FCString::Atoi(*hitActor->Tags[2].ToString());
            ClickObjectType = FCString::Atoi(*hitActor->Tags[1].ToString());
            workcitizen = BuildManager->buildingWorkCount[ObjectId];
            joblessCitizen = Network->playerjobless;
            ResourceUI = false;
            BuildingUI = true;
        }
        else if (hitActor->ActorHasTag("Army"))
        {
            UE_LOG(LogTemp, Log, TEXT("Army"));
            ObjectType = ARMYSTART;
            ObjectId = FCString::Atoi(*hitActor->Tags[2].ToString());

            ResourceUI = false;
            BuildingUI = false;
        }
        else
        {
            ObjectId = -1;
            ObjectType = 0;
            ResourceUI = false;
            BuildingUI = false;
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
    }
}

void AMyPlayerController::SelectBuildingHouse2()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 2;
    }
}

void AMyPlayerController::SelectBuildingHouse3()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 3;
    }
}

void AMyPlayerController::SelectBuildingHunterHouse()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 11;
    }
}

void AMyPlayerController::SelectBuildingARMYCAMP()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 21;
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
            ResourceActor = hitActor;
            ObjectType = RESOURCESTART;
            ObjectId = FCString::Atoi(*hitActor->Tags[1].ToString());
            ClickObjectType = ResourceManager->resource_type[ObjectId];
            ResourceCount = ResourceManager->resource_amount[ObjectId];
            workcitizen = ResourceManager->workCitizens[ObjectId];
            joblessCitizen = Network->playerjobless;
        }
        else if (BuildingUI == true)
        {
            joblessCitizen = Network->playerjobless;
            ObjectId = FCString::Atoi(*hitActor->Tags[2].ToString());
            workcitizen = BuildManager->buildingWorkCount[ObjectId];
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
