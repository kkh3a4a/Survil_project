#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "../../../../IOCPServer/protocol.h"
#include "Engine/DecalActor.h"
#include "MeshTerrain.generated.h"

class FTerrainThread : public FRunnable
{
public:
	FTerrainThread();
	void Stop();
	virtual uint32_t Run() override;

	bool LineX = false;
	bool LineY = false;

	char TerrainLineX[SIGHT_X];
	char TerrainLineY[SIGHT_Y];

	float x;
	float y;

	AMeshTerrain* TerrainClass;
	bool Running = true;
private:


};

UCLASS()
class PROJECT_API AMeshTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshTerrain();
	~AMeshTerrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
		UMaterialInstance* TerrainMaterialInstance;

	UMaterialInstanceDynamic* TerrainMaterialInstanceDynamic;

	int8 Terrain2DArray[SIGHT_X][SIGHT_Y];

	TArray<FVector> Vertices;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* MeshTerrain;


	FRunnableThread* WorkThread;
	FTerrainThread* Work;
	bool ReadyToUpdate = false;

	void ChangeWindDirection(int x, int y);
};
