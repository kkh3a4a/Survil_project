// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Main.h"
#include "NetworkingThread.h"
#include "ResourceManager.h"
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
    InputComponent->BindAction("1", IE_Pressed, this, &AMyPlayerController::SelectBuilding1);
    InputComponent->BindAction("2", IE_Pressed, this, &AMyPlayerController::SelectBuilding2);
    InputComponent->BindAction("3", IE_Pressed, this, &AMyPlayerController::SelectBuilding3);
    InputComponent->BindAction("4", IE_Pressed, this, &AMyPlayerController::SelectBuilding4);

    InputComponent->BindAction("MouseScrollUp", IE_Pressed, this, &AMyPlayerController::MouseScrollUp);
    InputComponent->BindAction("MouseScrollDown", IE_Pressed, this, &AMyPlayerController::MouseScrollDown);
}

void AMyPlayerController::InputUpPressed()
{
    Key_w = true;
    SendMovePacket();
}

void AMyPlayerController::InputDownPressed()
{
    Key_s = true;
    SendMovePacket();
}

void AMyPlayerController::InputLeftPressed()
{
    Key_a = true;
    SendMovePacket();
}

void AMyPlayerController::InputRightPressed()
{
    Key_d = true;
    SendMovePacket();
}



void AMyPlayerController::InputUpReleased()
{
    Key_w = false;
    SendMovePacket();
}

void AMyPlayerController::InputDownReleased()
{
    Key_s = false;
    SendMovePacket();
}

void AMyPlayerController::InputLeftReleased()
{
    Key_a = false;
    SendMovePacket();
}

void AMyPlayerController::InputRightReleased()
{
    Key_d = false;
    SendMovePacket();
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
        BuildManager->Build();
    }
    else {
        MoveToMouseCursor();
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
        }
        else if (hitActor->ActorHasTag("Resource"))
        {
            ResourceActor = hitActor;
            ResourceUI = true;
            ObjectType = RESOURCESTART;
            ObjectId = FCString::Atoi(*hitActor->Tags[1].ToString());
            ResourceType = ResourceManager->resource_type[ObjectId];
            ResourceCount = ResourceManager->resource_amount[ObjectId];
            workcitizen = ResourceManager->workCitizens[ObjectId];
            joblessCitizen = ResourceManager->playerjobless;
        }
        else
        {
            ObjectId = -1;
            ObjectType = 0;
            ResourceUI = false;
        }
    }
}

void AMyPlayerController::MoveToActor()
{
  
}

void AMyPlayerController::VisibilityTemperature()
{
    ATemperature* TemperatureClass = Main_Class->Temperature;

    UE_LOG(LogTemp, Log, TEXT("Thermal"));

    bool Hidden = TemperatureClass->GetIsHidden();
    if (Hidden) {
        TemperatureClass->Hide(false);
	}
    else{
        TemperatureClass->Hide(true);
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

void AMyPlayerController::SelectBuilding1()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 1;
    }
}

void AMyPlayerController::SelectBuilding2()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 2;
    }
}

void AMyPlayerController::SelectBuilding3()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 3;
    }
}

void AMyPlayerController::SelectBuilding4()
{
    if (BuildManager->BuildMode) {
        BuildManager->SelectedBuilding = 4;
    }
}

void AMyPlayerController::MouseScrollUp()
{
   if (Main_Class->MyCamera->GetActorLocation().Z <= 1500)
        return;
    Main_Class->MyCamera->SetActorLocation(Main_Class->MyCamera->GetActorLocation() - FVector(0, 100, 500));
    Main_Class->MyCamera->SetActorRotation(FRotator(Main_Class->MyCamera->GetActorRotation().Pitch + 2, Main_Class->MyCamera->GetActorRotation().Yaw, Main_Class->MyCamera->GetActorRotation().Roll));
}

void AMyPlayerController::MouseScrollDown()
{
    if (Main_Class->MyCamera->GetActorLocation().Z >= 7000)
        return;
    Main_Class->MyCamera->SetActorLocation(Main_Class->MyCamera->GetActorLocation() + FVector(0, 100, 500));
    Main_Class->MyCamera->SetActorRotation(FRotator(Main_Class->MyCamera->GetActorRotation().Pitch - 2, Main_Class->MyCamera->GetActorRotation().Yaw, Main_Class->MyCamera->GetActorRotation().Roll));
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if(Network == nullptr)
    {
        Network = reinterpret_cast<FSocketThread*>(Main_Class->Network);
    }

    mouse_cnt++;
    //UE_LOG(LogTemp, Log, TEXT("%s : %lf, %lf"),  *(ServerClass->MouseInput.name), ServerClass->MouseInput.location.x, ServerClass->MouseInput.location.y);
    //오른쪽 클릭 작업
    if (ResourceUI == true)
    {
        ResourceActor = hitActor;        
        ObjectType = RESOURCESTART;
        ObjectId = FCString::Atoi(*hitActor->Tags[1].ToString());
        ResourceType = ResourceManager->resource_type[ObjectId];
        ResourceCount = ResourceManager->resource_amount[ObjectId];
        workcitizen = ResourceManager->workCitizens[ObjectId];
        joblessCitizen = ResourceManager->playerjobless;
    }
    
    if (ResourceActor != NULL)
    {
        if (ResourceUI)
        {
           
        }
    }
    if (BuildManager->BuildMode) {
        OnBuildMode();
    }
}

void AMyPlayerController::SendMovePacket()
{
    cs_packet_move packet;
    packet.w = Key_w;
    packet.a = Key_a;
    packet.s = Key_s;
    packet.d = Key_d;
    packet.size = sizeof(cs_packet_move);
    packet.type = CS_PACKET_MOVE;
    UE_LOG(LogTemp, Log, TEXT("send Move"));
    WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, packet.size, &packet);
    WSASend(Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}
