// Fill out your copyright notice in the Description page of Project Settings.


#include "RSRelicTreasureChest.h"
#include "GameFramework/GameModeBase.h"
#include "RSSpawnManagerAccessor.h"

ARSRelicTreasureChest::ARSRelicTreasureChest()
{
	InteractName = FText::FromString(TEXT("유물 보물 상자"));
}

void ARSRelicTreasureChest::OpenChest()
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

	TArray<FName> UnspawnedRelics = SpawnManager->GetUnspawnedRelics().Array();

	if (0 >= UnspawnedRelics.Num())
	{
		return;
	}

	int32 RandIndex = FMath::RandRange(0, UnspawnedRelics.Num() - 1);

	if (UnspawnedRelics.IsValidIndex(RandIndex))
	{
		FTransform TargetTransform = GetActorTransform();
		FVector TargetLocation = TargetTransform.GetLocation();
		TargetLocation.Z += 75;

		TargetTransform.SetLocation(TargetLocation);

		SpawnManager->SpawnGroundRelicAtTransform(UnspawnedRelics[RandIndex], TargetTransform);
	}
}
