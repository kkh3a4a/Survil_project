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
    InputComponent->BindAction("RightClick", IE_Pressed, this, &AMyPlayerController::InputRightMoustButtonPressed);

    InputComponent->BindAction("RightClick", IE_Released, this, &AMyPlayerController::InputRightMoustButtonReleased);

    InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMyPlayerController::InputLeftMoustButtonPressed);

    InputComponent->BindAction("LeftClick", IE_Released, this, &AMyPlayerController::InputLeftMoustButtonReleased);
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
        DestLocation = Hit.ImpactPoint;
        hitActor = Hit.GetActor();
        UE_LOG(LogTemp, Log, TEXT("%s : %lf, %lf"), "a", DestLocation.X, DestLocation.Y);
    }
}

void AMyPlayerController::MoveToActor()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        DestLocation = Hit.ImpactPoint;
        if (hitActor) {
            if (hitActor->ActorHasTag("Citizen"))
            {
                hitActor->SetActorLocation(DestLocation);
                UE_LOG(LogTemp, Log, TEXT("%s : %lf, %lf"), "b", DestLocation.X, DestLocation.Y);
                hitActor = NULL;
            }
        }

    }
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    //오른쪽 클릭 작업
    if (bLeftClickMouse)
    {
        MoveToMouseCursor();
    }
    else if (bRightClickMouse)
    {
        MoveToActor();
    }

}