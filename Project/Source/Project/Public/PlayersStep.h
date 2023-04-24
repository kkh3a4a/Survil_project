// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayersStep.generated.h"

UCLASS()
class PROJECT_API APlayersStep : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayersStep();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HopePercent")
	float HopePercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComplaintPercent")
	float ComplaintPercent = 0.0f;
};
