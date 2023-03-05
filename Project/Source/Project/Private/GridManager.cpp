// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"

// Sets default values
AGridManager::AGridManager()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	PopulateGrid();
}

void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridManager::PopulateGrid()
{
	GridArray = {};
	FVector vector;
	FTransform transform;

	// Calculate offset
	WorldOffset = (GridSize * WorldGridSize * 0.5) - (WorldGridSize * 0.5);

	for (int i = 0; i < GridSize; ++i)
	{
		for (int j = 0; j < GridSize; ++j)
		{
			vector.X = j * WorldGridSize - WorldOffset;
		}
		vector.Y = i * WorldGridSize - WorldOffset;
	}
	
	transform.SetLocation(vector);

	GridArray.Add(GetWorld()->SpawnActor<AActor>(GridCell, transform));
}

FVector AGridManager::GetClossetGridPosition(FVector InPosition)
{
	FVector OutputVector;
	FVector ClossetPosition;
	float ClossetDistance;

	ClossetPosition = GridArray[0]->GetActorLocation();

	ClossetDistance = FVector::Dist(ClossetPosition, ClossetPosition);

	for (AActor* ArrayElement : GridArray)
	{
		if (FVector::Dist(ArrayElement->GetActorLocation(), InPosition) < ClossetDistance)
		{
			ClossetPosition = ArrayElement->GetActorLocation();

			ClossetDistance = FVector::Dist(ArrayElement->GetActorLocation(), InPosition);
		}

		OutputVector = ClossetPosition;
	}

	return OutputVector;
}

