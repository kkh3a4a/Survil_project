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

	BuildingArray.Add(Building_1);
	BuildingArray.Add(Building_2);
	BuildingArray.Add(Building_3);
	BuildingArray.Add(Building_4);
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
	cs_packet_build Packet;
	Packet.building_type = SelectedBuilding;
	Packet.x = DecalLocation.X;
	Packet.y = DecalLocation.Y;
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
