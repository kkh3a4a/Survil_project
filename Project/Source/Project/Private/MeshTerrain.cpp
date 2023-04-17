#include "MeshTerrain.h"

// Sets default values
AMeshTerrain::AMeshTerrain()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// Set the root component as the root of the actor
	RootComponent = Root;
	// Set up the mesh component
	MeshTerrain = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshTerrain"));
}

AMeshTerrain::~AMeshTerrain()
{
	if (Work != nullptr)
		Work->Stop();
}

void AMeshTerrain::BeginPlay()
{
	Super::BeginPlay();

	for (int32 Y = 0; Y < SIGHT_Y; Y++) {
		for (int32 X = 0; X < SIGHT_X; X++) {
			FVector Vertex(100 * X, 100 * Y, 0);
			Vertices.Add(Vertex);
		}
	}
	TArray<int32> Triangles;
	for (int32 Y = 0; Y < SIGHT_Y - 1; Y++) {
		for (int32 X = 0; X < SIGHT_X - 1; X++) {
			Triangles.Add(Y * SIGHT_X + X);
			Triangles.Add((Y + 1) * SIGHT_X + X);
			Triangles.Add(Y * SIGHT_X + X + 1);

			Triangles.Add((Y + 1) * SIGHT_X + (X + 1));
			Triangles.Add(Y * SIGHT_X + X + 1);
			Triangles.Add((Y + 1) * SIGHT_X + X);
		}
	}
	MeshTerrain = NewObject<UProceduralMeshComponent>(this);
	MeshTerrain->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);

	MeshTerrain->SetMaterial(0, TerrainMaterialInstance);
	MeshTerrain->RegisterComponent();

	SetRootComponent(RootComponent);
	MeshTerrain->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Work = new FTerrainThread();
	Work->TerrainClass = this;
	WorkThread = FRunnableThread::Create(Work, TEXT("TerrainThread"), 0, TPri_BelowNormal);
}

void AMeshTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!ReadyToUpdate) {
		return;
	}

	FVector Location = GetActorLocation();
	bool _MoveX = Work->MoveX;
	bool _MoveY = Work->MoveY;
	float _X = Work->x;
	float _Y = Work->y;
	if (_MoveX && _MoveY) {
		SetActorLocation(FVector(_X, _Y, 0.0));
		UE_LOG(LogTemp, Warning, TEXT("LOG FROM MESHTERRAIN.CPP XY %d"), ticki);
	}
	else if (_MoveX) {
		SetActorLocation(FVector( _X, Location.Y, 0.0));
		UE_LOG(LogTemp, Warning, TEXT("LOG FROM MESHTERRAIN.CPP X  %d"), ticki);
	}
	else if (_MoveY) {
		SetActorLocation(FVector(Location.X,  _Y, 0.0));
		UE_LOG(LogTemp, Warning, TEXT("LOG FROM MESHTERRAIN.CPP  Y %d"), ticki);

	}
	ticki++;
	Work->MoveX = false;
	Work->MoveY = false;

	for (int32 i = 0; i < Vertices.Num(); i++) {
		Vertices[i].Z = Terrain2DArray[i % SIGHT_X][i / SIGHT_X] * 50;
	}
	MeshTerrain->UpdateMeshSection_LinearColor(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>());
	ReadyToUpdate = false;
}

FTerrainThread::FTerrainThread()
{
}

void FTerrainThread::Stop()
{
	Running = false;
}

uint32_t FTerrainThread::Run()
{
	UE_LOG(LogTemp, Warning, TEXT("Terrain Thread Start"));
	while (1) {
		if (!Running)
			break;

		if (LineX) {
			static float TerrainX = x;
			if ((int)TerrainX != (int)x) {
				if ((int)TerrainX < (int)x) {
					memcpy(TerrainClass->Terrain2DArray[0], TerrainClass->Terrain2DArray[1], SIGHT_Y * (SIGHT_X - 1));
					memcpy(TerrainClass->Terrain2DArray[SIGHT_X - 1], TerrainLineY, SIGHT_Y);
				}
				if ((int)TerrainX > (int)x) {
					memcpy(TerrainClass->Terrain2DArray[1], TerrainClass->Terrain2DArray[0], SIGHT_Y * (SIGHT_X - 1));
					memcpy(TerrainClass->Terrain2DArray[0], TerrainLineY, SIGHT_Y);
				}
				TerrainX = x;
			}
			LineX = false;
			MoveX = true;
		}
		if (LineY) {
			static float TerrainY = y;
			if ((int)TerrainY != (int)y) {
				if ((int)TerrainY < (int)y) {
					for (int i = 0; i < SIGHT_X - 1; ++i) {
						for (int j = 0; j < SIGHT_Y - 1; ++j)
							TerrainClass->Terrain2DArray[i][j] = TerrainClass->Terrain2DArray[i][j + 1];
					}
					for (int i = 0; i < SIGHT_X; ++i) {
						TerrainClass->Terrain2DArray[i][SIGHT_Y - 1] = TerrainLineX[i];
					}
				}
				if ((int)TerrainY > (int)y) {
					for (int i = 0; i < SIGHT_X - 1; ++i) {
						for (int j = SIGHT_Y - 1; j > 0; --j) {
							TerrainClass->Terrain2DArray[i][j] = TerrainClass->Terrain2DArray[i][j - 1];
						}
					}
					for (int i = 0; i < SIGHT_X; ++i) {
						TerrainClass->Terrain2DArray[i][0] = TerrainLineX[i];
					}
				}
				TerrainY = y;
			}
			LineY = false;
			MoveY = true;
		}
		if(MoveX || MoveY)
			TerrainClass->ReadyToUpdate = true;
		
	}
	UE_LOG(LogTemp, Warning, TEXT("Terrain Thread Dead"));
	return 0;
}
