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

void AMeshTerrain::InitializeMeshTerrain(UMaterial* TerrainMaterial)
{
	for (int32 Y = 0; Y < map_size; Y++)
	{
		for (int32 X = 0; X < map_size; X++)
		{
			FVector Vertex(100 * X, 100 * Y, 0);
			Vertices.Add(Vertex);
		}
	}
	TArray<int32> Triangles;
	for (int32 Y = 0; Y < map_size - 1; Y++)
	{
		for (int32 X = 0; X < map_size - 1; X++)
		{
			Triangles.Add(Y * map_size + X);
			Triangles.Add((Y + 1) * map_size + X);
			Triangles.Add(Y * map_size + X + 1);

			Triangles.Add((Y + 1) * map_size + (X + 1));
			Triangles.Add(Y * map_size + X + 1);
			Triangles.Add((Y + 1) * map_size + X);
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

void AMeshTerrain::UpdateMeshTerrain(TArray<TArray<int8>> Terrain2DArray)
{
	for (int32 i = 0; i < Vertices.Num(); i++) {
		Vertices[i].Z = Terrain2DArray[i % map_size][i / map_size] * 50;
	}
	MeshTerrain->UpdateMeshSection_LinearColor(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>());
}
