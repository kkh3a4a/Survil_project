// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Server_testing.h"
#include "GridManager.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

AServer_testing* ServerClass;
AGridManager* GridManagerClass;
TSubclassOf<AGridManager> GridManager;

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
    ServerClass = Cast<AServer_testing>(server);
    //ServerClass = Cast<AServer_testing>(server->GetClass());

    if (ServerClass == nullptr)
    {
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

}

void AMyPlayerController::InputUpPressed()
{
    ServerClass->my_key_input->w = true;
}

void AMyPlayerController::InputDownPressed()
{
    ServerClass->my_key_input->s = true;
}

void AMyPlayerController::InputLeftPressed()
{
    ServerClass->my_key_input->a = true;
}

void AMyPlayerController::InputRightPressed()
{
    ServerClass->my_key_input->d = true;
}



void AMyPlayerController::InputUpReleased()
{
    ServerClass->my_key_input->w = false;
}

void AMyPlayerController::InputDownReleased()
{
    ServerClass->my_key_input->s = false;
}

void AMyPlayerController::InputLeftReleased()
{
    ServerClass->my_key_input->a = false;
}

void AMyPlayerController::InputRightReleased()
{
    ServerClass->my_key_input->d = false;
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
                ServerClass->Citizens->Citizen_moving->team = FCString::Atoi(*hitActor->Tags[1].ToString());
                ServerClass->Citizens->Citizen_moving->citizen_number = FCString::Atoi(*hitActor->Tags[2].ToString());
                ServerClass->Citizens->Citizen_moving->location.x = hitActor->GetActorLocation().X;
                ServerClass->Citizens->Citizen_moving->location.y = hitActor->GetActorLocation().Y;
                ServerClass->Citizens->Citizen_moving->location.z = hitActor->GetActorLocation().Z;
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
            ResourceCount = ServerClass->MyTown->resources_create_landscape[(FCString::Atoi(*ResourceActor->Tags[2].ToString()))]->count;
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
            ServerClass->Citizens->Citizen_moving->citizen_job = 1;
            DestLocation = Hit.GetActor()->GetActorLocation();

            UE_LOG(LogTemp, Log, TEXT("Resource"));
        }
        else
        {
            ServerClass->Citizens->Citizen_moving->citizen_job = 0;
        }

        if (hitActor) {
            if (hitActor->ActorHasTag("Citizen"))
            {
                temped = true;
                ServerClass->Citizens->Citizen_moving->location.x = DestLocation.X;
                ServerClass->Citizens->Citizen_moving->location.y = DestLocation.Y;
                ServerClass->Citizens->Citizen_moving->location.z = DestLocation.Z;
                hitActor = NULL;
            }
        }
        
        
        
        ResourceUI = false;


        mouse_end_t = high_resolution_clock::now();
    }
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
           
            ServerClass->Citizens->Citizen_moving->team = -1;
            temped = false;

        }
    }
    if (ResourceActor != NULL)
    {
        ResourceCount = ServerClass->MyTown->resources_create_landscape[(FCString::Atoi(*ResourceActor->Tags[2].ToString()))]->count;

        CitizenCount = ServerClass->MyTown->resources_create_landscape[(FCString::Atoi(*ResourceActor->Tags[2].ToString()))]->CitizenCount;

        if (ResourceUI)
        {
                ServerClass->UI_Input.resouce_input.ResourceNum = FCString::Atoi(*ResourceActor->Tags[2].ToString());
                ServerClass->UI_Input.resouce_input.CitizenCountAdd = CitizenAdd;
                ServerClass->UI_Input.resouce_input.CitizenCountSub = CitizenSub;
                UE_LOG(LogTemp, Log, TEXT("sub"));
        }
    }

    // building 관련
    if (PlacementModeEnabled == true)
    {
        UpdatePlacement();
    }
}

void AMyPlayerController::SetPlacementModeEnabled(bool IsEnabled)
{
    UE_LOG(LogTemp, Log, TEXT("Set"));

    if (IsEnabled == PlacementModeEnabled)
    {
        return;
    }
    else
    {
        IsEnabled = PlacementModeEnabled;
        if (IsEnabled == true)
        {
            FVector Location(0.0f, 0.0f, 0.0f);
            FRotator Rotation(0.0f, 0.0f, 0.0f);
            FActorSpawnParameters SpawnParams;

            StartList = ServerClass->players_list;
            for (int i = 0; i < MAXPLAYER; ++i)
            {
                Location = { StartList[i]->location.x, StartList[i]->location.y, StartList[i]->location.z };
                PlaceableActor = GetWorld()->SpawnActor<AActor>(PlaceableActorType, Location, Rotation, SpawnParams);
            }

            if (IsValid(PlaceableActor->GetComponentByClass(ClickableComponent)))
            {
                PlaceableActor->DestroyConstructedComponents();
                
                PlaceableActor->AddComponentByClass(PloppableComponent, true, PlaceableActor->GetActorTransform(), false);
            }
            else
            {
                PlaceableActor->AddComponentByClass(PloppableComponent, true, PlaceableActor->GetActorTransform(), false);
            }
        }
        else
        {
            if (IsValid(PlaceableActor))
            {
                GetWorld()->DestroyActor(PlaceableActor);
            }
        }
    }
}

void AMyPlayerController::UpdatePlacement()
{
    FHitResult Hit;
    FCollisionQueryParams TraceParams;
    FVector MouseLoc, MouseDir;
    
    DeprojectMousePositionToWorld(MouseLoc, MouseDir);

    MouseDir *= 100000.0;
    MouseDir += MouseLoc;

    GetWorld()->LineTraceSingleByChannel(Hit, MouseLoc, MouseDir, ECC_Visibility, TraceParams);

    if (Hit.bBlockingHit)
    {
        UGameplayStatics::GetActorOfClass(GetWorld(), GridManager);
        
        PlaceableActor->SetActorLocation(GridManagerClass->GetClossetGridPosition(Hit.Location));
    }
}

void AMyPlayerController::SpawnBuilding()
{
    if (IsValid(PlaceableActor->GetComponentByClass(PloppableComponent)))
    {
        //if(PloppableComponent.Get(IsPlacementValid)
        // Actor의 경우 

        FActorSpawnParameters spawnParams;
       
        GetWorld()->SpawnActor<AActor>(PlaceableActorType, PlaceableActor->GetActorTransform(), spawnParams);
        // 위 Transform()정보.
        
        SetPlacementModeEnabled(false);
    }
}

