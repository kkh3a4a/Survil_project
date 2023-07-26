// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Misc/DateTime.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Research.generated.h"

UCLASS()
class PROJECT_API AResearch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResearch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int count_lab = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int tech_phase = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int tech1 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int tech2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int tech3 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int tech4 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int tech5 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float tech_timer = 0.f;
	class FSocketThread* _NetWork;


public:

	void set_NetWork(void* NetWork);

	UFUNCTION(BlueprintCallable)
		void Send_Technology(int type, int level);

	UFUNCTION(BlueprintCallable)
		void Send_Techphase(int phase);


	void set_technology(int type, int level);
	void set_tech_phase(int phase);
	// Called every frame
	
};
