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

public:
	ADecalActor* DecalActor;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
		UMaterial* BuildingGridMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building_WoodenSign;				//type : 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building01_HOUSE;				//type : 1 / house
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building02_OIL_DRILL;			//type : 2 / oil drill
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building03_WATER_DRILL;			//type : 3 / water drill
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building04_SAW_MILL;				//type : 4 / saw mill
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building05_STEEL_MILL;			//type : 5 / steel mill
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building06_LABORATORY;			//type : 6 / laboratory
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building07_WAREHOUSE;			//type : 7 / warehouse
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building08_SPRINKLER;				//type : 8 / sprinkler
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building09_MEDICAL_CENTER;		//type : 9 / medical center
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building10_ARMY_ARMYCAMP;			//type : 10 / army camp
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building11_HUNTERHOUSE;			//type : 11 / hunterhouose->guard post
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building12_EXCHANGE_STATION;		//type : 12 / exchange station
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building13_GREEN_HOUSE;			//type : 13 / green house
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor>Building14_FACTORY;				//type : 14 / factory


	UMaterialInstanceDynamic* MaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
