// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Army.generated.h"

UCLASS()
class PROJECT_API AArmy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArmy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 state;	// 0 : 서있기, 1 : 이동, 2 : 공격, 3 : 사망

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Army_HP {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float  Army_MAXHP {100};

};
