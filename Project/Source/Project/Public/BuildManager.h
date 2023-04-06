#pragma once
#include <math.h>
#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
		UMaterial* BuildingGridMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
		TSubclassOf<AActor>Building_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
		TSubclassOf<AActor>Building_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
		TSubclassOf<AActor>Building_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
		TSubclassOf<AActor>Building_4;


	bool BuildMode = false;
	FVector DecalLocation;
	char SelectedBuilding{};

	void UpdateDecalPosition(FVector, float, float);
	void DecalVisibility();
	void Build();

	TArray<TSubclassOf<AActor>*> BuildingArray;
	TArray<AActor*> BuildedBuildings;
};
