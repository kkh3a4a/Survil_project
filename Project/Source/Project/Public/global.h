// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/Vector.h"
#include <string>
#include"global.generated.h"

/**
 *
 */


#define MAXPLAYER 1


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


USTRUCT(Atomic, BlueprintType)
struct Fcitizen_struct {
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FActor_location_rotation> citizen_location_rotation;

};

USTRUCT(Atomic, BlueprintType)
struct FAActor_struct {
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> citizen_AActor;

};


USTRUCT(Atomic, BlueprintType)
struct FCitizen_moving
{
	GENERATED_USTRUCT_BODY()
public:
	int team;
	int citizen_number;
	Fthree_float location;
	Fthree_float rotation;
};

class PROJECT_API global
{
public:
	global();
	~global();
};

