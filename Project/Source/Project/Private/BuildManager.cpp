#include "BuildManager.h"
#include "Kismet/GameplayStatics.h"

#define CITYSIZE 100

// Sets default values
ABuildManager::ABuildManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABuildManager::BeginPlay()
{
	Super::BeginPlay();

	FVector Location = FVector(0, 0, 0);
	DecalActor = GetWorld()->SpawnActor<ADecalActor>(Location, FRotator(0.f, -90.f, 0.f));
	DecalActor->SetActorScale3D(FVector(10.f, 2.f, 2.f));

	UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(BuildingGridMaterial, this);

	DecalActor->SetDecalMaterial(MaterialInstance);
	DecalActor->SetActorHiddenInGame(true);

	BuildingArray.Add(Building_1);
	BuildingArray.Add(Building_2);
	BuildingArray.Add(Building_3);
	BuildingArray.Add(Building_4);
}

void ABuildManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DecalActor->SetActorLocation(DecalLocation);
}

void ABuildManager::DecalVisibility()
{
	if (BuildMode) {
		DecalActor->SetActorHiddenInGame(false);
	}
	else {
		DecalActor->SetActorHiddenInGame(true);
	}
	SelectedBuilding = 0;
}

void ABuildManager::Build()
{
	if (SelectedBuilding < 1 || SelectedBuilding > BuildingArray.Num()) {
		UE_LOG(LogTemp, Log, TEXT("Select Type of Building!!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Selected Building %d"), SelectedBuilding);
	FRotator Rotation = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo;
	ABuilding* BuildingActor = GetWorld()->SpawnActor<ABuilding>(DecalLocation, Rotation, SpawnInfo);
	BuildingActor->SetMesh(BuildingArray[SelectedBuilding - 1]);

	BuiltBuildings.Add(BuildingActor);
	UE_LOG(LogTemp, Log, TEXT("Built Buildings: %d"), BuiltBuildings.Num());
}

void ABuildManager::UpdateDecalPosition(FVector MouseHitPoint, float CityX, float CityY)
{
	FVector CalculatedLocation;
	CalculatedLocation.X = std::min((int64)CityX + (int64)(CITYSIZE * 100 / 2), (int64)MouseHitPoint.X);
	CalculatedLocation.X = std::max((int64)CityX - (int64)(CITYSIZE * 100 / 2), (int64)CalculatedLocation.X);
	CalculatedLocation.Y = std::min((int64)CityY + (int64)(CITYSIZE * 100 / 2), (int64)MouseHitPoint.Y);
	CalculatedLocation.Y = std::max((int64)CityY - (int64)(CITYSIZE * 100 / 2), (int64)CalculatedLocation.Y);
	CalculatedLocation = FVector((uint64)CalculatedLocation.X / 1000 * 1000 + 500, (uint64)CalculatedLocation.Y / 1000 * 1000 + 500, 0);
	DecalLocation = CalculatedLocation;
}
