// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Main.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AMain* Main_Class;

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

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableTouchEvents = true;
    bEnableMouseOverEvents = true;
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("RightClick", IE_Pressed, this, &AMyPlayerController::InputRightMoustButtonPressed);
    InputComponent->BindAction("RightClick", IE_Released, this, &AMyPlayerController::InputRightMoustButtonReleased);

    InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMyPlayerController::InputLeftMoustButtonPressed);
    InputComponent->BindAction("LeftClick", IE_Released, this, &AMyPlayerController::InputLeftMoustButtonReleased);

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

    InputComponent->BindAction("MouseScrollUp", IE_Pressed, this, &AMyPlayerController::MouseScrollUp);
    InputComponent->BindAction("MouseScrollDown", IE_Pressed, this, &AMyPlayerController::MouseScrollDown);
}

void AMyPlayerController::InputUpPressed()
{
    Main_Class->KeyInput.w = true;
}

void AMyPlayerController::InputDownPressed()
{
    Main_Class->KeyInput.s = true;
}

void AMyPlayerController::InputLeftPressed()
{
    Main_Class->KeyInput.a = true;
}

void AMyPlayerController::InputRightPressed()
{
    Main_Class->KeyInput.d = true;
}



void AMyPlayerController::InputUpReleased()
{
    Main_Class->KeyInput.w = false;
}

void AMyPlayerController::InputDownReleased()
{
    Main_Class->KeyInput.s = false;
}

void AMyPlayerController::InputLeftReleased()
{
    Main_Class->KeyInput.a = false;
}

void AMyPlayerController::InputRightReleased()
{
    Main_Class->KeyInput.d = false;
}



void AMyPlayerController::InputRightMoustButtonPressed()
{
    bRightClickMouse = true;
}

void AMyPlayerController::InputRightMoustButtonReleased()
{
    bRightClickMouse = false;
}

void AMyPlayerController::InputLeftMoustButtonPressed()
{
    bLeftClickMouse = true;
}

void AMyPlayerController::InputLeftMoustButtonReleased()
{
    bLeftClickMouse = false;
}

void AMyPlayerController::MoveToMouseCursor()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        hitActor = Hit.GetActor();
        if (hitActor->ActorHasTag("Citizen"))
        {
            UE_LOG(LogTemp, Log, TEXT("Citizen"));
            if (wcscmp(*hitActor->Tags[1].ToString(), L"0") == 0)
            {
                Main_Class->Citizens->Citizen_moving->Team = FCString::Atoi(*hitActor->Tags[1].ToString());
                Main_Class->Citizens->Citizen_moving->CitizenNumber = FCString::Atoi(*hitActor->Tags[2].ToString());
                Main_Class->Citizens->Citizen_moving->Location.x = hitActor->GetActorLocation().X;
                Main_Class->Citizens->Citizen_moving->Location.y = hitActor->GetActorLocation().Y;
                Main_Class->Citizens->Citizen_moving->Location.z = hitActor->GetActorLocation().Z;
                // UE_LOG(LogTemp, Log, TEXT("%d %d %lf, %lf"), ServerClass->Citizen_moving->team, ServerClass->Citizen_moving->citizen_number ,ServerClass->Citizen_moving->location.x, ServerClass->Citizen_moving->location.y);
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
            ResourceType = FCString::Atoi(*hitActor->Tags[1].ToString());
            ResourceCount = Main_Class->MyTown->resources_create_landscape[(FCString::Atoi(*ResourceActor->Tags[2].ToString()))]->Count;
        }
        else
        {
            ResourceUI = false;
        }
    }
}

void AMyPlayerController::MoveToActor()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);
    if (Hit.bBlockingHit)
    {
        UE_LOG(LogTemp, Log, TEXT("checkmouse"));
        DestLocation = Hit.ImpactPoint;
        if (Hit.GetActor()->ActorHasTag(L"Resource"))
        {
            Main_Class->Citizens->Citizen_moving->Job = 1;
            DestLocation = Hit.GetActor()->GetActorLocation();

            UE_LOG(LogTemp, Log, TEXT("Resource"));
        }
        else
        {
            Main_Class->Citizens->Citizen_moving->Job = 0;
        }

        if (hitActor) {
            if (hitActor->ActorHasTag("Citizen"))
            {
                temped = true;
                Main_Class->Citizens->Citizen_moving->Location.x = DestLocation.X;
                Main_Class->Citizens->Citizen_moving->Location.y = DestLocation.Y;
                Main_Class->Citizens->Citizen_moving->Location.z = DestLocation.Z;
                hitActor = NULL;
            }
        }
        ResourceUI = false;
        mouse_end_t = high_resolution_clock::now();
    }
}

void AMyPlayerController::VisibilityTemperature()
{
    ATemperature* TemperatureClass = Main_Class->Temperature;
    UE_LOG(LogTemp, Log, TEXT("Thermal"));
    bool Hidden = TemperatureClass->GetIsHidden();
    if (Hidden) {
        TemperatureClass->Hide(false);
        Main_Class->SunManager->SetActorHiddenInGame(true);
	}
    else{
        TemperatureClass->Hide(true);
        Main_Class->SunManager->SetActorHiddenInGame(false);
	}
}

void AMyPlayerController::BuildMode()
{
    if (Main_Class->Building->BuildMode) {
        Main_Class->Building->BuildMode = false;
        UE_LOG(LogTemp, Log, TEXT("BuildMode Off"));
    }
    else {
        Main_Class->Building->BuildMode = true;
        UE_LOG(LogTemp, Log, TEXT("BuildMode On"));
    }
    Main_Class->Building->DecalVisibility();
}

void AMyPlayerController::OnBuildMode()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);
    if (Hit.bBlockingHit)
    {
        FVector CalculatedLocation;
        CalculatedLocation.X = min((int64)Main_Class->Players[0]->CityLocation.x + (int64)(CITYSIZE * 100 / 2), (int64)Hit.ImpactPoint.X);
        CalculatedLocation.X = max((int64)Main_Class->Players[0]->CityLocation.x - (int64)(CITYSIZE * 100 / 2), (int64)CalculatedLocation.X);
        CalculatedLocation.Y = min((int64)Main_Class->Players[0]->CityLocation.y + (int64)(CITYSIZE * 100 / 2), (int64)Hit.ImpactPoint.Y);
        CalculatedLocation.Y = max((int64)Main_Class->Players[0]->CityLocation.y - (int64)(CITYSIZE * 100 / 2), (int64)CalculatedLocation.Y);
        CalculatedLocation = FVector((uint64)CalculatedLocation.X / 1000 * 1000 + 500, (uint64)CalculatedLocation.Y / 1000 * 1000 + 500, 0);
        /*UE_LOG(LogTemp, Log, TEXT("LOC: %lld %lld"), (int64)Main_Class->ServerStruct1.PlayerInfo.location.x, (int64)Main_Class->ServerStruct1.PlayerInfo.location.y);
        UE_LOG(LogTemp, Log, TEXT("X: %lld %lld"), (int64)Main_Class->ServerStruct1.PlayerInfo.location.x - (int64)(CITYSIZE * 100 / 2), (int64)Main_Class->ServerStruct1.PlayerInfo.location.x + (int64)(CITYSIZE * 100 / 2));
        UE_LOG(LogTemp, Log, TEXT("Y: %lld %lld"), (int64)Main_Class->ServerStruct1.PlayerInfo.location.y - (int64)(CITYSIZE * 100 / 2), (int64)Main_Class->ServerStruct1.PlayerInfo.location.y + (int64)(CITYSIZE * 100 / 2));
        UE_LOG(LogTemp, Log, TEXT("%lld %lld %lld"), (uint64)CalculatedLocation.X, (uint64)CalculatedLocation.Y, 0);*/
        Main_Class->Building->DecalLocation = CalculatedLocation;

        if (bLeftClickMouse) {
            UE_LOG(LogTemp, Log, TEXT("click on build mode"));
        }
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

    mouse_cnt++;
    //UE_LOG(LogTemp, Log, TEXT("%s : %lf, %lf"),  *(ServerClass->MouseInput.name), ServerClass->MouseInput.location.x, ServerClass->MouseInput.location.y);
    
    if (Main_Class->Building->BuildMode) {
        OnBuildMode();
    }
    else if (bLeftClickMouse) {     //왼쪽 마우스 클릭 작업
        MoveToMouseCursor();
    }
    else if (bRightClickMouse) {    //오른쪽 마우스 클릭 작업
        MoveToActor();
    }
    
    mouse_start_t = high_resolution_clock::now();
    if (temped && (mouse_cnt % 100 == 0))
    {
        if (duration_cast<milliseconds>(mouse_start_t - mouse_end_t).count() > 1000)
        {
            Main_Class->Citizens->Citizen_moving->Team = -1;
            temped = false;
        }
    }
    if (ResourceActor != NULL)
    {
        ResourceCount = Main_Class->MyTown->resources_create_landscape[(FCString::Atoi(*ResourceActor->Tags[2].ToString()))]->Count;
        CitizenCount = Main_Class->MyTown->resources_create_landscape[(FCString::Atoi(*ResourceActor->Tags[2].ToString()))]->CitizenCount;

        if (ResourceUI)
        {
            if (Main_Class->RecvedUIInput == true)
            {
                Main_Class->UI_Input.ResourceInput.CitizenCountAdd = false;
                Main_Class->UI_Input.ResourceInput.CitizenCountSub = false;
                if (CitizenRelease)
                {
                    Main_Class->CitizenRelaese = CitizenRelease;
                    CitizenRelease = false;
                }
            }
            else
            {                
                Main_Class->UI_Input.ResourceInput.ResourceNum = FCString::Atoi(*ResourceActor->Tags[2].ToString());
                Main_Class->UI_Input.ResourceInput.CitizenCountAdd = CitizenAdd;
                Main_Class->UI_Input.ResourceInput.CitizenCountSub = CitizenSub;
                Main_Class->CitizenRelaese = CitizenRelease;
            }
        }
    }
    
}
