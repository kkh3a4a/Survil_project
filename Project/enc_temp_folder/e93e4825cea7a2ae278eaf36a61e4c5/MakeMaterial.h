// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <iostream>
#include <random>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialExpressionTextureSampleParameter2D.h"
#include "Materials/MaterialExpression.h"
#include "Materials/Material.h"
#include "Materials/MaterialExpressionMaterialFunctionCall.h"
#include "Materials/MaterialExpressionFunctionInput.h"
#include "Materials/MaterialFunction.h"
#include "Materials/MaterialFunctionInstance.h"
#include "Materials/MaterialExpressionFunctionOutput.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionMultiply.h"
#include "Materials/MaterialExpressionConstant4Vector.h"
#include "Materials/MaterialExpressionAdd.h"


#include "MakeMaterial.generated.h"

UCLASS()
class PROJECT_API AMakeMaterial : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMakeMaterial();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		TSubclassOf<AActor>terrain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
		UMaterial* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Function")
		UMaterialFunction* Func;

	
	int NumOfDots = 20;
	float _DecalSize = NumOfDots + 2;
	float _Space = 1.f / _DecalSize;
	float _Radius = 3.f / (_DecalSize * 2);
	float _Density = 1.5f;

	TArray<UMaterialExpressionMultiply*> Multiplies;
	UMaterialInstanceDynamic* MaterialInstance;

	void MakeMaterial();
	void GetMaterial();
};
