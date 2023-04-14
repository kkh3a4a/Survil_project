#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "../../../../IOCPServer/protocol.h"
#include "MeshTerrain.generated.h"


UCLASS()
class PROJECT_API AMeshTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshTerrain();

protected:

public:	

	void InitializeMeshTerrain(UMaterialInstance* TerrainMaterial);
	void UpdateMeshTerrain(int8(*Terrain2DArrayPtr)[SIGHT_Y]);

	TArray<FVector> Vertices;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* MeshTerrain;
};
