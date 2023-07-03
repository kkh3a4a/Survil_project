// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Research2.generated.h"

UCLASS()
class PROJECT_API AResearch2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResearch2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int research_level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<bool> clicked;

	UFUNCTION(BlueprintCallable)
	bool is_enable_level(int level);

	UFUNCTION(BlueprintCallable)
	bool is_enable_level_button(int level);

	UFUNCTION(BlueprintCallable)
	void clicked_button(int index);

	UFUNCTION(BlueprintCallable)
	bool is_enable_button(int level, int clicked_index, bool ex_upgrade);
};
