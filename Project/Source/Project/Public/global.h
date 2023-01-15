// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"global.generated.h"
/**
 *
 */

USTRUCT(Atomic, BlueprintType)
struct Fthree_float {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float x;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float z;
};

USTRUCT(Atomic, BlueprintType)
struct FActor_location_rotation {
	GENERATED_USTRUCT_BODY()
public:
	TCHAR name[30];

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Fthree_float location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Fthree_float rotation;
};
class PROJECT_API global
{
public:
	global();
	~global();
};

