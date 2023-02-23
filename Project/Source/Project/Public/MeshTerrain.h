#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "MeshTerrain.generated.h"

const int MapSizeX = 200;
const int MapSizeY = 120;

UCLASS()
class PROJECT_API AMeshTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshTerrain();

protected:

public:	

	void InitializeMeshTerrain(UMaterial* TerrainMaterial);
	void UpdateMeshTerrain(TArray<TArray<int8>> Terrain2DArray);
	

	UInstancedStaticMeshComponent* InstancedTerrain;

	TArray<FVector> Vertices;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* MeshTerrain;
};
