// Fill out your copyright notice in the Description page of Project Settings.


#include "RSWeaponTreasureChest.h"
#include "GameFramework/GameModeBase.h"
#include "RSSpawnManagerAccessor.h"

ARSWeaponTreasureChest::ARSWeaponTreasureChest()
{
	InteractName = FText::FromString(TEXT("무기 보물 상자"));
}

void ARSWeaponTreasureChest::OpenChest()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	IRSSpawnManagerAccessor* SpawnManagerAccessor = World->GetAuthGameMode<IRSSpawnManagerAccessor>();
	URSSpawnManager* SpawnManager = nullptr;

	if (SpawnManagerAccessor)
	{
		SpawnManager = SpawnManagerAccessor->GetSpawnManager();
	}

	if (!SpawnManager)
	{
		return;
	}

	TArray<FName> UnSpawnedWeapons = SpawnManager->GetUnspawnedWeapons().Array();

	if (0 >= UnSpawnedWeapons.Num())
	{
		return;
	}

	int32 RandIndex = FMath::RandRange(0, UnSpawnedWeapons.Num() - 1);

	if (UnSpawnedWeapons.IsValidIndex(RandIndex))
	{
		FTransform TargetTransform = GetActorTransform();
		FVector TargetLocation = TargetTransform.GetLocation();
		TargetLocation.Z += 75;

		TargetTransform.SetLocation(TargetLocation);

		SpawnManager->SpawnGroundWeaponAtTransform(UnSpawnedWeapons[RandIndex], TargetTransform, false);
	}
}
