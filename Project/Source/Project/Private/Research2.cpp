// Fill out your copyright notice in the Description page of Project Settings.


#include "Research2.h"

// Sets default values
AResearch2::AResearch2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AResearch2::BeginPlay()
{
	Super::BeginPlay();
	
	clicked.Init(0, 30);
}


bool AResearch2::is_enable_level(int level)
{
	if (research_level >= level)
	{
		return true;
	}

	return false;
}

bool AResearch2::is_enable_level_button(int level)
{
	if (research_level + 1 == level)
	{
		return true;
	}

	return false;
}

void AResearch2::clicked_button(int index)
{
	clicked[index] = true;

}

bool AResearch2::is_enable_button(int level, int clicked_index, bool ex_upgrade)
{
	if (research_level >= level && clicked[clicked_index] == false && ex_upgrade == true)
	{
		return true;
	}

	return false;
}
