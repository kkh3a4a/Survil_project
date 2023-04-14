#include "MeshTerrain.h"

// Sets default values
AMeshTerrain::AMeshTerrain()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// Set the root component as the root of the actor
	RootComponent = Root;
	// Set up the mesh component
	MeshTerrain = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshTerrain"));
}

void AMeshTerrain::InitializeMeshTerrain(UMaterialInstance* TerrainMaterial)
{
	for (int32 Y = 0; Y < SIGHT_Y; Y++){
		for (int32 X = 0; X < SIGHT_X; X++){
			FVector Vertex(100 * X, 100 * Y, 0);
			Vertices.Add(Vertex);
		}
	}
	TArray<int32> Triangles;
	for (int32 Y = 0; Y < SIGHT_Y - 1; Y++){
		for (int32 X = 0; X < SIGHT_X - 1; X++){
			Triangles.Add(Y * SIGHT_X + X);
			Triangles.Add((Y + 1) * SIGHT_X + X);
			Triangles.Add(Y * SIGHT_X + X + 1);

			Triangles.Add((Y + 1) * SIGHT_X + (X + 1));
			Triangles.Add(Y * SIGHT_X + X + 1);
			Triangles.Add((Y + 1) * SIGHT_X + X);
		}
	}
	// Create the mesh component
	MeshTerrain = NewObject<UProceduralMeshComponent>(this);
	MeshTerrain->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	
	// Add the mesh component to the actor
	MeshTerrain->SetMaterial(0, TerrainMaterial);
	MeshTerrain->RegisterComponent();
	
	SetRootComponent(RootComponent);
	MeshTerrain->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AMeshTerrain::UpdateMeshTerrain(int8(*Terrain2DArrayPtr)[SIGHT_Y])
{
	for (int32 i = 0; i < Vertices.Num(); i++) {
		Vertices[i].Z = Terrain2DArrayPtr[i % SIGHT_X][i / SIGHT_X] * 50;
		//UE_LOG(LogTemp, Log, TEXT("%d"), Vertices[i].Z);
	}
	MeshTerrain->UpdateMeshSection_LinearColor(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>());
}
