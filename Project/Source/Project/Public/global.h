// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/Vector.h"
#include <string>
#include"global.generated.h"

#define MAXPLAYER 1
#define MAXCITIZEN 200
#define FIRSTSPAWN 10
#define MAPSIZEX 200
#define MAPSIZEY 120
#define CITYSIZE 100

USTRUCT(Atomic, BlueprintType)
struct FTwoInt {
	GENERATED_USTRUCT_BODY()
public:
	int x{};
	int y{};

};

USTRUCT(Atomic, BlueprintType)
struct FThreeFloat {
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
struct FCitizenSole {
	GENERATED_USTRUCT_BODY()
public:
	TCHAR Name[30];
	FTwoInt Location{};
	int Rotation{};
	char Resources[5];
	char HP{};
	char Job{};
	char IsJob{};
	FTwoInt JobLocation{};
};

USTRUCT(Atomic, BlueprintType)
struct FCitizenMoving
{
	GENERATED_USTRUCT_BODY()
public:
	int Team;
	int CitizenNumber;
	FThreeFloat Location;
	FThreeFloat Rotation;
	int Job; //////////////// 0 : 무직, 1 : 자원 채취
};

USTRUCT(Atomic, BlueprintType)
struct FResource
{
	GENERATED_USTRUCT_BODY()
public:
	int Type{};		///////////////0 : 석유c,		1 : 물,		2 : 철,		3 : 식량,	4 : 나무
	int Count{};
	FTwoInt Location{};
	int CitizenCount{};
};

USTRUCT(Atomic, BlueprintType)
struct FBuildingInfo
{
	GENERATED_USTRUCT_BODY()
public:
	char Type{};
	char Level{};
	FTwoInt Location{};
	char HP{};
	char IncludeCitizen{};
};

USTRUCT(Atomic, BlueprintType)
struct FKeyInput
{
	GENERATED_USTRUCT_BODY()
public:
	bool w;
	bool a;
	bool s;
	bool d;
};

USTRUCT(Atomic, BlueprintType)
struct FUI_resource_Input {
	GENERATED_USTRUCT_BODY()
public:
	int ResourceNum;
	int CitizenCountAdd;
	int CitizenCountSub;
};

USTRUCT(Atomic, BlueprintType)
struct FUI_Input {
	GENERATED_USTRUCT_BODY()
public:
	FUI_resource_Input ResourceInput;
};

USTRUCT(Atomic, BlueprintType)
struct FPlayerInfo {
	GENERATED_USTRUCT_BODY()
public:
	FTwoInt CityLocation{};
	FCitizenSole Citizen[100]{};
	FResource Resource[5]{};
	FBuildingInfo Building[20]{};
};

class PROJECT_API global
{
public:
	global();
	~global();
};

