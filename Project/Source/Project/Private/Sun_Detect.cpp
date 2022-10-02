// Fill out your copyright notice in the Description page of Project Settings.

#include "Sun_Detect.h"

// Sets default values
ASun_Detect::ASun_Detect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASun_Detect::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASun_Detect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//태양과의 각도

	//UE_LOG(LogTemp, Log, TEXT("%lf"), a);



	//태양의 밝기
	

	//그림자가 있는지


}
