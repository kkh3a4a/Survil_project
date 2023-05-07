#pragma once
#include <math.h>
#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "GameFramework/Actor.h"
#include "Building.h"
#include "NetworkingThread.h"
#include "BuildManager.generated.h"

UCLASS()
class PROJECT_API ABuildManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	ADecalActor* DecalActor;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
		UMaterial* BuildingGridMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building_WoodenSign;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building_HOUSE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building_HOUSE2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building_HOUSE3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building_HUNTERHOUSE;

	UMaterialInstanceDynamic* MaterialInstance;
	bool BuildMode = false;
	bool Buildable = false;

	FVector DecalLocation;
	int SelectedBuilding{};

	void UpdateDecalPosition(FVector, float, float);
	void DecalVisibility();
	void Build(int, float, float, int);
	void BuildSuccess(int obj_id, float x, float y, int building_type);
	void SendBuildablePacket();
	void SendBuildPacket();
	void SetBuildingPlacement(int Building_id, char work_citizen);
	class AMain* Main;
	TMap<int,TSubclassOf<AActor>> BuildingArray;
	AActor* BuiltBuildings[MAXBUILDING];
	int buildingWorkCount[MAXBUILDING] = {};
};
