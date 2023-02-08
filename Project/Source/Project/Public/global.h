// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/Vector.h"
#include <string>
#include"global.generated.h"

/**
 *
 */


#define MAXPLAYER 2


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
struct FCitizen_sole {
	GENERATED_USTRUCT_BODY()
public:
	TCHAR name[30];

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Fthree_float location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Fthree_float rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int resource_type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int resource_count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int job;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int isJob;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	Fthree_float Job_location;
};


USTRUCT(Atomic, BlueprintType)
struct Fcitizen_struct {
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FCitizen_sole> citizen_location_rotation;

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
	int citizen_job; //////////////// 0 : 무직, 1 : 자원 채취
};

USTRUCT(Atomic, BlueprintType)
struct Fresources_actor
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int type;		///////////////0 : 석유,		1 : 물,		2 : 철,		3 : 식량,	4 : 나무
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	Fthree_float location;
};

USTRUCT(Atomic, BlueprintType)
struct Fkeyboard_input
{
	GENERATED_USTRUCT_BODY()
public:
	bool w;
	bool a;
	bool s;
	bool d;
};

class PROJECT_API global
{
public:
	global();
	~global();
};

