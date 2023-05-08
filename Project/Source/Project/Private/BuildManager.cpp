#include "BuildManager.h"
#include "Main.h"
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

	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMain::StaticClass());
	if (actor == nullptr) {
		return;
	}
	actor->GetWorld();
	Main = Cast<AMain>(actor);
	if (Main == nullptr) {
		return;
	}

	FVector Location = FVector(0, 0, 0);
	DecalActor = GetWorld()->SpawnActor<ADecalActor>(Location, FRotator(0.f, -90.f, 0.f));
	DecalActor->SetActorScale3D(FVector(10.f, 2.f, 2.f));

	MaterialInstance = UMaterialInstanceDynamic::Create(BuildingGridMaterial, this);
	MaterialInstance->SetVectorParameterValue(TEXT("Color"), FLinearColor(0, 1, 0, 1));

	DecalActor->SetDecalMaterial(MaterialInstance);
	DecalActor->SetActorHiddenInGame(true);

	BuildingArray.Add(0, Building_WoodenSign);
	BuildingArray.Add(1, Building_HOUSE);
	BuildingArray.Add(2, Building_HOUSE2);
	BuildingArray.Add(3, Building_HOUSE3);
	BuildingArray.Add(11, Building_HUNTERHOUSE);

	for (auto& a : BuiltBuildings)
	{
		a = nullptr;
	}
}

void ABuildManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BuildMode) {
		DecalActor->SetActorLocation(DecalLocation);
		SendBuildablePacket();
		if (Buildable) {
			MaterialInstance->SetVectorParameterValue(TEXT("Color"), FLinearColor(0, 1, 0, 1));
		}
		else {
			MaterialInstance->SetVectorParameterValue(TEXT("Color"), FLinearColor(1, 0, 0, 1));
		}
	}
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

void ABuildManager::Build(int obj_id, float x , float y, int building_type)
{
	UE_LOG(LogTemp, Log, TEXT("Building %d %d"), building_type, obj_id);
	 UWorld* uworld = GetWorld();
    if (uworld == nullptr)
        return;

	FActorSpawnParameters SpawnInfo;
	if(BuiltBuildings[obj_id] == nullptr)
	{
		AActor * building = uworld->SpawnActor<AActor>(BuildingArray[0], FVector(x, y, 0.f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
		BuiltBuildings[obj_id] = building;
	}
}

void ABuildManager::BuildSuccess(int obj_id, float x, float y, int building_type)
{
	UE_LOG(LogTemp, Log, TEXT("Building Success %d %d"), building_type, obj_id);
	UWorld* uworld = GetWorld();
	if (uworld == nullptr)
		return;
	FActorSpawnParameters SpawnInfo;
	BuiltBuildings[obj_id]->Destroy();
	BuiltBuildings[obj_id] = nullptr;

	AActor* building = uworld->SpawnActor<AActor>(BuildingArray[building_type], FVector(x, y, 0.f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	BuiltBuildings[obj_id] = building;
	BuiltBuildings[obj_id]->Tags.Add(TEXT("Building"));
	BuiltBuildings[obj_id]->Tags.Add(*FString::FromInt(building_type));
	BuiltBuildings[obj_id]->Tags.Add(*FString::FromInt(obj_id));
}

void ABuildManager::SendBuildablePacket()
{
	cs_packet_buildable Packet;
	Packet.building_type = SelectedBuilding;
	Packet.x = DecalLocation.X;
	Packet.y = DecalLocation.Y;
	WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, Packet.size, &Packet);
	WSASend(Main->Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}

void ABuildManager::SendBuildPacket()
{
	if (!BuildingArray.Find(SelectedBuilding)) {
		UE_LOG(LogTemp, Log, TEXT("Select Type of Building!!"));
		return;
	}
	if (!Buildable) {
		UE_LOG(LogTemp, Log, TEXT("Cannot build here!!"));
		return;
	}
	cs_packet_build Packet;
	Packet.building_type = SelectedBuilding;
	Packet.x = DecalLocation.X;
	Packet.y = DecalLocation.Y;
	WSA_OVER_EX* wsa_over_ex = new WSA_OVER_EX(OP_SEND, Packet.size, &Packet);
	WSASend(Main->Network->s_socket, &wsa_over_ex->_wsabuf, 1, 0, 0, &wsa_over_ex->_wsaover, send_callback);
}

void ABuildManager::SetBuildingPlacement(int Building_id, char work_citizen)
{
	buildingWorkCount[Building_id] = work_citizen;
}

void ABuildManager::UpdateDecalPosition(FVector MouseHitPoint, float CityX, float CityY)
{
	FVector CalculatedLocation;
	CalculatedLocation.X = min((int64)CityX + (int64)(CITYSIZE * 100 / 2), (int64)MouseHitPoint.X);
	CalculatedLocation.X = max((int64)CityX - (int64)(CITYSIZE * 100 / 2), (int64)CalculatedLocation.X);
	CalculatedLocation.Y = min((int64)CityY + (int64)(CITYSIZE * 100 / 2), (int64)MouseHitPoint.Y);
	CalculatedLocation.Y = max((int64)CityY - (int64)(CITYSIZE * 100 / 2), (int64)CalculatedLocation.Y);
	CalculatedLocation = FVector((uint64)CalculatedLocation.X / 1000 * 1000 + 500, (uint64)CalculatedLocation.Y / 1000 * 1000 + 500, 0);
	DecalLocation = CalculatedLocation;
}
