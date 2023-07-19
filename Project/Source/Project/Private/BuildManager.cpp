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
	DecalActor->SetActorScale3D(FVector(10.f, 1.6f, 1.6f));

	MaterialInstance = UMaterialInstanceDynamic::Create(BuildingGridMaterial, this);
	MaterialInstance->SetVectorParameterValue(TEXT("Color"), FLinearColor(0, 1, 0, 1));

	DecalActor->SetDecalMaterial(MaterialInstance);
	DecalActor->SetActorHiddenInGame(true);

	BuildingArray.Add(0, Building_WoodenSign);
	BuildingArray.Add(1, Building01_HOUSE);
	BuildingArray.Add(2, Building02_OIL_DRILL);
	BuildingArray.Add(3, Building03_WATER_DRILL);
	BuildingArray.Add(4, Building04_SAW_MILL);
	BuildingArray.Add(5, Building05_STEEL_MILL);
	BuildingArray.Add(6, Building06_LABORATORY);
	BuildingArray.Add(7, Building07_WAREHOUSE);
	BuildingArray.Add(8, Building08_SPRINKLER);
	BuildingArray.Add(9, Building09_MEDICAL_CENTER);
	BuildingArray.Add(21, Building21_ARMY_CAMP);
	BuildingArray.Add(11, Building11_HUNTERHOUSE);
	BuildingArray.Add(12, Building12_EXCHANGE_STATION);
	BuildingArray.Add(13, Building13_GREEN_HOUSE);
	BuildingArray.Add(14, Building14_FACTORY);

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

		if (SelectedBuilding == 8)
		{
			DecalActor->SetActorLocation({ DecalLocation.X - 500, DecalLocation.Y - 500, DecalLocation.Z });
		}

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

	if (BuiltBuildings[obj_id] == nullptr)
	{
		AActor* building = uworld->SpawnActor<AActor>(BuildingArray[0], FVector(x, y, 0.f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
		BuiltBuildings[obj_id] = building;
	}
}

void ABuildManager::BuildSuccess(int obj_id, float x, float y, int building_type)
{
	UE_LOG(LogTemp, Log, TEXT("Building Success %d %d"), building_type, obj_id);
	UWorld* uworld = GetWorld();
	if (uworld == nullptr)
		return;
	else {
		FActorSpawnParameters SpawnInfo;
		BuiltBuildings[obj_id]->Destroy();
		BuiltBuildings[obj_id] = nullptr;

		AActor* building = uworld->SpawnActor<AActor>(BuildingArray[building_type], FVector(x, y, 0.f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
		BuiltBuildings[obj_id] = building;
		BuiltBuildings[obj_id]->Tags.Add(TEXT("Building"));
		BuiltBuildings[obj_id]->Tags.Add(*FString::FromInt(building_type));
		BuiltBuildings[obj_id]->Tags.Add(*FString::FromInt(obj_id));
		BuiltBuildings[obj_id]->Tags.Add(*FString::FromInt(obj_id / PLAYERBUILDINGCOUNT));
		if (building_type == 8) {
			BuiltBuildings[obj_id]->Tags.Add(TEXT("OFF"));
		}
		const TArray<FName>& ObjectTags = BuiltBuildings[obj_id]->Tags;
		for (const FName& Tag : ObjectTags)
		{
			UE_LOG(LogTemp, Log, TEXT("%s"), *Tag.ToString());
		}
	}
}

void ABuildManager::SendBuildablePacket()
{
	cs_packet_buildable Packet;
	Packet.building_type = SelectedBuilding;
	Packet.x = DecalLocation.X;
	Packet.y = DecalLocation.Y;

	if(SelectedBuilding == 8)
	{
		Packet.x -= 500;
		Packet.y -= 500;
	}
	
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

	if (Packet.building_type == 8)
	{
		Packet.x -= 500;
		Packet.y -= 500;
	}
	
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
