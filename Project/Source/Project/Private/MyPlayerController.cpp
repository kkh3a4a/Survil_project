// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Server_testing.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AServer_testing* ServerClass;

AMyPlayerController::AMyPlayerController()
{

    UWorld* worldref = GetWorld();
    if (worldref == nullptr){
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("%s"), *worldref->GetName());
    
    AActor* actor = UGameplayStatics::GetActorOfClass(worldref, AServer_testing::StaticClass());
    if (actor == nullptr){
        return;
    }
    actor->GetWorld();
    ServerClass = Cast<AServer_testing>(actor);
    if (ServerClass == nullptr){
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

    InputComponent->BindAction("MouseScrollUp", IE_Pressed, this, &AMyPlayerController::MouseScrollUp);
    InputComponent->BindAction("MouseScrollDown", IE_Pressed, this, &AMyPlayerController::MouseScrollDown);
}

void AMyPlayerController::InputUpPressed()
{
    ServerClass->KeyInput->w = true;
}

void AMyPlayerController::InputDownPressed()
{
    ServerClass->KeyInput->s = true;
}

void AMyPlayerController::InputLeftPressed()
{
    ServerClass->KeyInput->a = true;
}

void AMyPlayerController::InputRightPressed()
{
    ServerClass->KeyInput->d = true;
}



void AMyPlayerController::InputUpReleased()
{
    ServerClass->KeyInput->w = false;
}

void AMyPlayerController::InputDownReleased()
{
    ServerClass->KeyInput->s = false;
}

void AMyPlayerController::InputLeftReleased()
{
    ServerClass->KeyInput->a = false;
}

void AMyPlayerController::InputRightReleased()
{
    ServerClass->KeyInput->d = false;
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
                ServerClass->Citizens->Citizen_moving->Team = FCString::Atoi(*hitActor->Tags[1].ToString());
                ServerClass->Citizens->Citizen_moving->CitizenNumber = FCString::Atoi(*hitActor->Tags[2].ToString());
                ServerClass->Citizens->Citizen_moving->Location.x = hitActor->GetActorLocation().X;
                ServerClass->Citizens->Citizen_moving->Location.y = hitActor->GetActorLocation().Y;
                ServerClass->Citizens->Citizen_moving->Location.z = hitActor->GetActorLocation().Z;
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
            ResourceCount = ServerClass->MyTown->resources_create_landscape[(FCString::Atoi(*ResourceActor->Tags[2].ToString()))]->Count;
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
            ServerClass->Citizens->Citizen_moving->Job = 1;
            DestLocation = Hit.GetActor()->GetActorLocation();

            UE_LOG(LogTemp, Log, TEXT("Resource"));
        }
        else
        {
            ServerClass->Citizens->Citizen_moving->Job = 0;
        }

        if (hitActor) {
            if (hitActor->ActorHasTag("Citizen"))
            {
                temped = true;
                ServerClass->Citizens->Citizen_moving->Location.x = DestLocation.X;
                ServerClass->Citizens->Citizen_moving->Location.y = DestLocation.Y;
                ServerClass->Citizens->Citizen_moving->Location.z = DestLocation.Z;
                hitActor = NULL;
            }
        }
        ResourceUI = false;
        mouse_end_t = high_resolution_clock::now();
    }
}

void AMyPlayerController::VisibilityTemperature()
{
    ATemperature* TemperatureClass = ServerClass->Temperature;

    UE_LOG(LogTemp, Log, TEXT("Thermal"));

    bool Hidden = TemperatureClass->GetIsHidden();
    if (Hidden) {
        TemperatureClass->Hide(false);
	}
    else{
        TemperatureClass->Hide(true);
	}
}

void AMyPlayerController::MouseScrollUp()
{
   if (ServerClass->MyCamera->GetActorLocation().Z <= 1500)
        return;
    ServerClass->MyCamera->SetActorLocation(ServerClass->MyCamera->GetActorLocation() - FVector(0, 100, 500));
    ServerClass->MyCamera->SetActorRotation(FRotator(ServerClass->MyCamera->GetActorRotation().Pitch + 2, ServerClass->MyCamera->GetActorRotation().Yaw, ServerClass->MyCamera->GetActorRotation().Roll));
}

void AMyPlayerController::MouseScrollDown()
{
    if (ServerClass->MyCamera->GetActorLocation().Z >= 7000)
        return;
    ServerClass->MyCamera->SetActorLocation(ServerClass->MyCamera->GetActorLocation() + FVector(0, 100, 500));
    ServerClass->MyCamera->SetActorRotation(FRotator(ServerClass->MyCamera->GetActorRotation().Pitch - 2, ServerClass->MyCamera->GetActorRotation().Yaw, ServerClass->MyCamera->GetActorRotation().Roll));
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    mouse_cnt++;
    //UE_LOG(LogTemp, Log, TEXT("%s : %lf, %lf"),  *(ServerClass->MouseInput.name), ServerClass->MouseInput.location.x, ServerClass->MouseInput.location.y);
    //오른쪽 클릭 작업
    if (bLeftClickMouse)
    {
        MoveToMouseCursor();
    }
    else if (bRightClickMouse)
    {
        MoveToActor();
    }
    
    mouse_start_t = high_resolution_clock::now();
    if (temped && (mouse_cnt % 100 == 0))
    {
        if (duration_cast<milliseconds>(mouse_start_t - mouse_end_t).count() > 1000)
        {
           
            ServerClass->Citizens->Citizen_moving->Team = -1;
            temped = false;

        }
    }
    if (ResourceActor != NULL)
    {
        ResourceCount = ServerClass->MyTown->resources_create_landscape[(FCString::Atoi(*ResourceActor->Tags[2].ToString()))]->Count;

        CitizenCount = ServerClass->MyTown->resources_create_landscape[(FCString::Atoi(*ResourceActor->Tags[2].ToString()))]->CitizenCount;

        if (ResourceUI)
        {
            if (ServerClass->RecvedUIInput == true)
            {
                ServerClass->UI_Input.ResourceInput.CitizenCountAdd = false;
                ServerClass->UI_Input.ResourceInput.CitizenCountSub = false;
                if (CitizenRelease)
                {
                    ServerClass->CitizenRelaese = CitizenRelease;
                    CitizenRelease = false;
                }
            }
            else
            {                
                ServerClass->UI_Input.ResourceInput.ResourceNum = FCString::Atoi(*ResourceActor->Tags[2].ToString());
                ServerClass->UI_Input.ResourceInput.CitizenCountAdd = CitizenAdd;
                ServerClass->UI_Input.ResourceInput.CitizenCountSub = CitizenSub;
                ServerClass->CitizenRelaese = CitizenRelease;
            }
        }
    }

}
