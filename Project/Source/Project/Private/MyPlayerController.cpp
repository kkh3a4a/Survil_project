// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "Server_testing.h"
#include "MyPlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

AServer_testing* server_MouseInput;

AMyPlayerController::AMyPlayerController()
{

    UWorld* worldref = GetWorld();
    if (worldref == nullptr)
    {
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("%s"), *worldref->GetName());
    server = UGameplayStatics::GetActorOfClass(worldref, AServer_testing::StaticClass());
    if (server == nullptr)
    {
        return;
    }
    server->GetWorld();
    server_MouseInput = Cast<AServer_testing>(server);
    //server_MouseInput = Cast<AServer_testing>(server->GetClass());

    if (server_MouseInput == nullptr)
    {
        return;
    }

    bShowMouseCursor = true;
    wcscpy(server_MouseInput->MouseInput.name, L"temp");
    server_MouseInput->MouseInput.location = { 0, 0, 0 };
    server_MouseInput->MouseInput.rotation = { 0, 0, 0 };
    //UE_LOG(LogTemp, Log, TEXT("%s : %f, %f"), *(server_MouseInput->MouseInput.name), server_MouseInput->MouseInput.location.x, server_MouseInput->MouseInput.location.y);
    //UE_LOG(LogTemp, Log, TEXT("%f, %f"), server_MouseInput->MouseInput.location.x, server_MouseInput->MouseInput.location.y);
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
        hitActor = Hit.GetActor();
        if (hitActor->ActorHasTag("Citizen"))
        {
            wcscpy(server_MouseInput->MouseInput.name, *(hitActor->GetName()));
            server_MouseInput->MouseInput.location.x = hitActor->GetActorLocation().X;
            server_MouseInput->MouseInput.location.y = hitActor->GetActorLocation().Y;
            server_MouseInput->MouseInput.location.z = hitActor->GetActorLocation().Z;
            UE_LOG(LogTemp, Log, TEXT("%s : %lf, %lf"), server_MouseInput->MouseInput.name, server_MouseInput->MouseInput.location.x, server_MouseInput->MouseInput.location.y);

        }
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
                server_MouseInput->MouseInput.location.x = DestLocation.X;
                server_MouseInput->MouseInput.location.y = DestLocation.Y;
                server_MouseInput->MouseInput.location.z = DestLocation.Z;
                hitActor = NULL;
            }
        }

    }
}


void AMyPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    //UE_LOG(LogTemp, Log, TEXT("%s : %lf, %lf"),  *(server_MouseInput->MouseInput.name), server_MouseInput->MouseInput.location.x, server_MouseInput->MouseInput.location.y);
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