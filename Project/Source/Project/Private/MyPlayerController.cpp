// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


AMyPlayerController::AMyPlayerController() 
{
	bShowMouseCursor = true;
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMyPlayerController::InputLeftMoustButtonPressed);

    InputComponent->BindAction("LeftClick", IE_Released, this, &AMyPlayerController::InputLeftMoustButtonReleased);
}

void AMyPlayerController::InputLeftMoustButtonPressed()
{
    bClickMouse = true;
}

void AMyPlayerController::InputLeftMoustButtonReleased()
{
    bClickMouse = false;
}

void AMyPlayerController::MoveToMouseCursor()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        DestLocation = Hit.ImpactPoint;
        UE_LOG(LogTemp, Log, TEXT("%lf, %lf"), DestLocation.X, DestLocation.Y);
    }
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if (bClickMouse)
    {
        MoveToMouseCursor();
    }
}