#include "BuildManager.h"
#include "Kismet/GameplayStatics.h"
#define CITYSIZE 100

// Sets default values
ABuildManager::ABuildManager()
{

}

void ABuildManager::BeginPlay()
{
	FVector Location = FVector(0, 0, 0);
	DecalActor = GetWorld()->SpawnActor<ADecalActor>(Location, FRotator(0.f, -90.f, 0.f));
	DecalActor->SetActorScale3D(FVector(10.f, 2.f, 2.f));

	UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(BuildingGridMaterial, this);

	DecalActor->SetDecalMaterial(MaterialInstance);
	DecalActor->SetActorHiddenInGame(true);

	BuildingArray.Add(&Building_1);
	BuildingArray.Add(&Building_2);
	BuildingArray.Add(&Building_3);
	BuildingArray.Add(&Building_4);
}

void ABuildManager::Tick(float DeltaTime)
{
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
	AActor* Building;
	Building = GetWorld()->SpawnActor<AActor>(*BuildingArray[SelectedBuilding - 1], DecalLocation, Rotation, SpawnInfo);
	BuildedBuildings.Add(Building);
	UE_LOG(LogTemp, Log, TEXT("Builded Buildings: %d"), BuildedBuildings.Num());
}

void ABuildManager::Update(FVector MouseHitPoint, float CityX, float CityY)
{
	//UE_LOG(LogTemp, Log, TEXT("X: %lld %lld"), (int64)MouseHitPoint.X - (int64)(CITYSIZE * 100 / 2), (int64)MouseHitPoint.Y);

	FVector CalculatedLocation;
	CalculatedLocation.X = std::min((int64)CityX + (int64)(CITYSIZE * 100 / 2), (int64)MouseHitPoint.X);
	CalculatedLocation.X = std::max((int64)CityX - (int64)(CITYSIZE * 100 / 2), (int64)CalculatedLocation.X);
	CalculatedLocation.Y = std::min((int64)CityY + (int64)(CITYSIZE * 100 / 2), (int64)MouseHitPoint.Y);
	CalculatedLocation.Y = std::max((int64)CityY - (int64)(CITYSIZE * 100 / 2), (int64)CalculatedLocation.Y);
	CalculatedLocation = FVector((uint64)CalculatedLocation.X / 1000 * 1000 + 500, (uint64)CalculatedLocation.Y / 1000 * 1000 + 500, 0);
	/*UE_LOG(LogTemp, Log, TEXT("LOC: %lld %lld"), (int64)Main_Class->ServerStruct1.PlayerInfo.location.x, (int64)Main_Class->ServerStruct1.PlayerInfo.location.y);
	UE_LOG(LogTemp, Log, TEXT("X: %lld %lld"), (int64)Main_Class->ServerStruct1.PlayerInfo.location.x - (int64)(CITYSIZE * 100 / 2), (int64)Main_Class->ServerStruct1.PlayerInfo.location.x + (int64)(CITYSIZE * 100 / 2));
	UE_LOG(LogTemp, Log, TEXT("Y: %lld %lld"), (int64)Main_Class->ServerStruct1.PlayerInfo.location.y - (int64)(CITYSIZE * 100 / 2), (int64)Main_Class->ServerStruct1.PlayerInfo.location.y + (int64)(CITYSIZE * 100 / 2));*/
	UE_LOG(LogTemp, Log, TEXT("%lld %lld"), (uint64)CityX, (uint64)CityY, 0);
	UE_LOG(LogTemp, Log, TEXT("%lld %lld"), (uint64)CalculatedLocation.X, (uint64)CalculatedLocation.Y, 0);
	DecalLocation = CalculatedLocation;
	DecalActor->SetActorLocation(DecalLocation);

}
