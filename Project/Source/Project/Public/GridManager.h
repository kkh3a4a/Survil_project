#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

UCLASS()
class PROJECT_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGridManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridSize = 32;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WorldGridSize = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WorldOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> GridCell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> GridArray;

	

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void PopulateGrid();

public:
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector GetClossetGridPosition(FVector InPosition);
};
