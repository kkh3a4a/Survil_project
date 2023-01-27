// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "servers_sand_test.generated.h"

UCLASS()
class PROJECT_API Aservers_sand_test : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aservers_sand_test();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 sand_scale { 5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> IntArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 sands_size {};
};
