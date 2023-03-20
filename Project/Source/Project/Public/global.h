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
struct FActorTransform {
	GENERATED_USTRUCT_BODY()
public:
	TCHAR name[30];

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FThreeFloat location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FThreeFloat rotation;
};

USTRUCT(Atomic, BlueprintType)
struct FCitizenSole {
	GENERATED_USTRUCT_BODY()
public:
	TCHAR Name[30];

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FThreeFloat Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FThreeFloat rotation;

	int Resources[5];

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Job;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int IsJob;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FThreeFloat Job_location;
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
struct Fresources_actor
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Type;		///////////////0 : 석유,		1 : 물,		2 : 철,		3 : 식량,	4 : 나무
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FThreeFloat Loaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CitizenCount = 0;

};

USTRUCT(Atomic, BlueprintType)
struct FBuildingInfo
{
	GENERATED_USTRUCT_BODY()
public:
	int Kind{};
	int Level{};
	int HP{};
	int IncludeCitizen{};
};

USTRUCT(Atomic, BlueprintType)
struct FKeyInput
{
	GENERATED_USTRUCT_BODY()
public:
	int w;
	int a;
	int s;
	int d;
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
struct FServerStruct1 {
	GENERATED_USTRUCT_BODY()
public:
	float SunAngle;
	FActorTransform PlayerInfo;
	int MyResource[5];
	FThreeFloat CurrentLocation;
};

USTRUCT(Atomic, BlueprintType)
struct FServerStruct2 {
	GENERATED_USTRUCT_BODY()
public:
	FCitizenSole PlayerCitizen[MAXPLAYER][MAXCITIZEN];
	Fresources_actor Resources[MAXPLAYER * 10];
};


USTRUCT(Atomic, BlueprintType)
struct FClientStruct1 {
	GENERATED_USTRUCT_BODY()
public:
	FCitizenMoving MyCitizenMoving = {};
	FKeyInput KeyInput = {};
	FUI_Input UIInput = {};
	int connecting = 1;
};

class PROJECT_API global
{
public:
	global();
	~global();
};

