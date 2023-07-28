// Fill out your copyright notice in the Description page of Project Settings.

#include "MakeMaterial.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Texture.h"
#include "PackageTools.h"
#include "AssetRegistryModule.h"
#include "Editor/UnrealEd/Public/PackageTools.h"
#include "Editor/UnrealEd/Public/FileHelpers.h"

// Sets default values
AMakeMaterial::AMakeMaterial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMakeMaterial::BeginPlay()
{
	Super::BeginPlay();

	if (Material)
		GetMaterial();
	else
		MakeMaterial();
}

// Called every frame
void AMakeMaterial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < NumOfDots * NumOfDots; i++) {
		//MaterialInstance->SetVectorParameterValue(*FString::Printf(TEXT("Color%d"), i), FLinearColor(i/_Size, 0, 0, 1));
		MaterialInstance->SetVectorParameterValue(*FString::Printf(TEXT("Color%d"), i), FLinearColor((float)(FMath::RandRange(0, 100)) / 100.f, (float)(FMath::RandRange(0, 100)) / 100.f, (float)(FMath::RandRange(0, 100)) / 100.f, 1));
	}
}

void AMakeMaterial::MakeMaterial()
{
	//머티리얼 생성
	FString MaterialName = TEXT("MyMaterial");
	FString PackageName = FString(TEXT("/Game/Materials/")) + MaterialName;

	// Create a material package
	UPackage* MaterialPackage = CreatePackage(NULL, *PackageName);
	MaterialPackage->AddToRoot();
	MaterialPackage->FullyLoad();

	//Create material
	Material = NewObject<UMaterial>(MaterialPackage, *MaterialName, RF_Public | RF_Standalone | RF_MarkAsRootSet);
	Material->BlendMode = EBlendMode::BLEND_Translucent;
	Material->MaterialDomain = EMaterialDomain::MD_DeferredDecal;
	for (int x = 0; x < NumOfDots; x++) {
		for (int y = 0; y < NumOfDots; y++) {
			// Create a material expression and set its properties
			UMaterialExpressionMaterialFunctionCall* FunctionCall = NewObject<UMaterialExpressionMaterialFunctionCall>(Material);
			FunctionCall->MaterialFunction = Func;
			Material->Expressions.Add(FunctionCall);
			Material->PostEditChange();

			TArray<FFunctionExpressionInput>& Inputs = FunctionCall->FunctionInputs;
			for (int32 j = 0; j < Inputs.Num(); j++)
			{
				if (j == 1) {
					// Set Center(V2)
					UMaterialExpressionVectorParameter* Center = NewObject<UMaterialExpressionVectorParameter>(Material);
					Center->DefaultValue = FLinearColor(_Space * x + (_Space * 1.5f), _Space * y + (_Space * 1.5f), 0.f);
					Center->ParameterName = FName("Center");
					Material->Expressions.Add(Center);
					Inputs[j].Input.Connect(0, Center);
				}
				else if (j == 2) {
					// Set Radius(S)
					UMaterialExpressionScalarParameter* Radius = NewObject<UMaterialExpressionScalarParameter>(Material);
					Radius->DefaultValue = _Radius;
					Radius->ParameterName = FName("Radius");
					Material->Expressions.Add(Radius);
					Inputs[j].Input.Connect(0, Radius);
				}
				else if (j == 3) {
					// Set Density(S)
					UMaterialExpressionScalarParameter* Density = NewObject<UMaterialExpressionScalarParameter>(Material);
					Density->DefaultValue = _Density;
					Density->ParameterName = FName("Density");
					Material->Expressions.Add(Density);
					Inputs[j].Input.Connect(0, Density);
				}
			}

			UMaterialExpressionVectorParameter* Color = NewObject<UMaterialExpressionVectorParameter>(Material);
			Color->ParameterName = FName("Color" + FString::FromInt(x * NumOfDots + y));
			Color->DefaultValue = FLinearColor(1.0f, 1.0f, 1.0f);
			Material->Expressions.Add(Color);

			UMaterialExpressionMultiply* MultiplyExpression = NewObject<UMaterialExpressionMultiply>(Material);
			Material->Expressions.Add(MultiplyExpression);

			MultiplyExpression->A.Connect(0, Color);
			MultiplyExpression->B.Connect(0, FunctionCall);

			Multiplies.Add(MultiplyExpression);
		}
	}

	UMaterialExpressionAdd* Add;
	UMaterialExpressionAdd* ParentAdd;

	Add = NewObject<UMaterialExpressionAdd>(Material);
	Material->Expressions.Add(Add);
	Add->A.Connect(0, Multiplies[0]);
	Add->B.Connect(0, Multiplies[1]);
	ParentAdd = Add;

	for (int i = 2; i < Multiplies.Num(); i++) {
		Add = NewObject<UMaterialExpressionAdd>(Material);
		Material->Expressions.Add(Add);
		Add->A.Connect(0, ParentAdd);
		Add->B.Connect(0, Multiplies[i]);
		ParentAdd = Add;
	}

	Material->EmissiveColor.Connect(0, ParentAdd);
	Material->PostEditChange();


	//저장
	MaterialPackage->SetDirtyFlag(true);
	MaterialPackage->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(Material);
	// Save the material package
	FString MaterialPackageName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	if (UPackage::SavePackage(MaterialPackage, Material, RF_Standalone, *MaterialPackageName)) {
		UE_LOG(LogTemp, Warning, TEXT("Material saved to %s"), *MaterialPackageName);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to save material to %s"), *MaterialPackageName);
	}

	//머티리얼 인스턴스 생성
	MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
	for (int i = 0; i < NumOfDots * NumOfDots; i++)
		MaterialInstance->SetVectorParameterValue(*FString::Printf(TEXT("Color%d"), i), FLinearColor(1, 0, 0, 1));


	//땅이랑 데칼 생성
	FVector Location(0, 0, 0);
	FRotator Rotation = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo;
	AActor* ground = GetWorld()->SpawnActor<AActor>(terrain, Location, Rotation, SpawnInfo);
	ground->SetActorScale3D(FVector(2000, 2000, 1));


	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 5; j++) {
			ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(Location + FVector(4000 * i, 4000 * j, 0), Rotation);
			UDecalComponent* DecalComponent = Decal->GetDecal();
			DecalComponent->DecalSize = FVector(100, 100, 100);
			Decal->SetActorScale3D(FVector(10, _DecalSize, _DecalSize));
			Decal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			Decal->SetDecalMaterial(MaterialInstance);
		}
	}
}

void AMakeMaterial::GetMaterial()
{
	//머티리얼 인스턴스 생성
	MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
	for (int i = 0; i < NumOfDots * NumOfDots; i++)
		MaterialInstance->SetVectorParameterValue(*FString::Printf(TEXT("Color%d"), i), FLinearColor(1, 0, 0, 1));


	//땅이랑 데칼 생성
	FVector Location(0, 0, 0);
	FRotator Rotation = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo;
	AActor* ground = GetWorld()->SpawnActor<AActor>(terrain, Location, Rotation, SpawnInfo);
	ground->SetActorScale3D(FVector(2000, 2000, 1));


	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 5; j++) {
			ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(Location + FVector(4000 * i, 4000 * j, 0), Rotation);
			UDecalComponent* DecalComponent = Decal->GetDecal();
			DecalComponent->DecalSize = FVector(100, 100, 100);
			Decal->SetActorScale3D(FVector(10, _DecalSize, _DecalSize));
			Decal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			Decal->SetDecalMaterial(MaterialInstance);
		}
	}
}
