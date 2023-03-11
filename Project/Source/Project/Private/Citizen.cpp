// Fill out your copyright notice in the Description page of Project Settings.


#include "Citizen.h"

// Sets default values
ACitizen::ACitizen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Citizen_moving = new FCitizenMoving;
	Citizen_moving->Team = -1;
	Citizen_moving->Location = { 0, 0, 0 };
	Citizen_moving->Rotation = { 0, 0, 0 };
	Citizen_moving->Job = 0;

}
void ACitizen::Initialize(TSubclassOf<AActor> C_Actor, TSubclassOf<AActor> E_Actor)
{
	Citizen_Actor = C_Actor;
	EnemyCitizenActor = E_Actor;
}

void ACitizen::Spawn_Citizen()
{
	AActor* Spawned_Citizen;
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	TArray<AActor*> New_Actors;
	int Is_Mycitizen = 0;
	for (auto& a : My_Citizen)
	{
		if (Is_Mycitizen == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("MY : %f %f %f"),a.Value[0].Location.x , a.Value[0].Location.y, a.Value[0].Location.z);
			for (int count_citizenNum = 0; count_citizenNum < 10; ++count_citizenNum)
			{
				Location = { a.Value[count_citizenNum].Location.x,a.Value[count_citizenNum].Location.y,a.Value[count_citizenNum].Location.z};
				Spawned_Citizen = GetWorld()->SpawnActor<AActor>(Citizen_Actor, Location, Rotation, SpawnInfo);
				Spawned_Citizen->Tags.Add("Citizen");
				Spawned_Citizen->Tags.Add(FName(*FString::FromInt(Is_Mycitizen)));
				Spawned_Citizen->Tags.Add(FName(*FString::FromInt(count_citizenNum)));
				New_Actors.Add(Spawned_Citizen);
			}
			Citizens_Editer.Add(Is_Mycitizen, New_Actors);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy : %f %f %f"), a.Value[0].Location.x, a.Value[0].Location.y, a.Value[0].Location.z);
			for (int count_citizenNum = 0; count_citizenNum < 10; ++count_citizenNum)
			{
				Location = { a.Value[count_citizenNum].Location.x, a.Value[count_citizenNum].Location.y, a.Value[count_citizenNum].Location.z };
				Spawned_Citizen = GetWorld()->SpawnActor<AActor>(EnemyCitizenActor, Location, Rotation, SpawnInfo);
				Spawned_Citizen->Tags.Add("Citizen");
				Spawned_Citizen->Tags.Add(FName(*FString::FromInt(Is_Mycitizen)));
				Spawned_Citizen->Tags.Add(FName(*FString::FromInt(count_citizenNum)));
				New_Actors.Add(Spawned_Citizen);
			}
			Citizens_Editer.Add(Is_Mycitizen, New_Actors);
		}
		Is_Mycitizen++;
		New_Actors.Empty();
	}
}

void ACitizen::Citizen_Moving()
{
	int team = 0;
	FVector Location(0.0f, 0.0f, 0.0f);
	for (auto& a : My_Citizen)
	{
		
		for (int count_citizenNum = 0; count_citizenNum < 10; ++count_citizenNum)
		{
			Location = { a.Value[count_citizenNum].Location.x,a.Value[count_citizenNum].Location.y,a.Value[count_citizenNum].Location.z };
			Citizens_Editer[team][count_citizenNum]->SetActorLocation(Location);
		}
		team++;
	}

}

void ACitizen::citizen_set(FServerStruct1& FirstSendServer, FServerStruct2& SecondSendServer)
{
	for (int i = 0; i < MAXPLAYER; ++i)
	{
		My_Citizen.Add(i, SecondSendServer.PlayerCitizen[i]);
	}
}



void ACitizen::TF_set(FThreeFloat& a, FThreeFloat& b)
{
	a.x = b.x;
	a.y = b.y;
	a.z = b.z;
}

void ACitizen::CitizenNoJob(int& cnt)
{
	int temp_cnt = 0;
	for (auto& a : My_Citizen)
	{
		for (int i = 0; i < 10; ++i)
		{
			if (a.Value[i].Job == 0)
			{
				temp_cnt++;
			}
		}
		break;
	}
	
	cnt = temp_cnt;
}