// Fill out your copyright notice in the Description page of Project Settings.


#include "RSLifeEssenceTreasureChest.h"
#include "GameFramework/GameModeBase.h"
#include "RSDungeonGameModeBase.h"
#include "RSSpawnManagerAccessor.h"

ARSLifeEssenceTreasureChest::ARSLifeEssenceTreasureChest()
{
	InteractName = FText::FromString(TEXT("생명의 정수 보물 상자"));
}

void ARSLifeEssenceTreasureChest::OpenChest()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	ARSDungeonGameModeBase* DungeonGameMode = World->GetAuthGameMode<ARSDungeonGameModeBase>();
	IRSSpawnManagerAccessor* SpawnManagerAccessor = Cast<IRSSpawnManagerAccessor>(DungeonGameMode);
	URSSpawnManager* SpawnManager = nullptr;

	if (SpawnManagerAccessor)
	{
		SpawnManager = SpawnManagerAccessor->GetSpawnManager();
	}

	if (!SpawnManager || !DungeonGameMode)
	{
		return;
	}

	int32 LevelIndex = DungeonGameMode->GetLevelIndex();
	LevelIndex += 1;

	int32 RandQuantity = FMath::RandRange(LevelIndex * 25, LevelIndex * 100);

	FTransform TargetTransform = GetActorTransform();
	FVector TargetLocation = TargetTransform.GetLocation();
	TargetLocation.Z += 75;

	TargetTransform.SetLocation(TargetLocation);

	for (int32 i = 0; i < RandQuantity; ++i)
	{
		FTimerHandle SpawnDelayTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(SpawnDelayTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
				if (SpawnManager)
				{
					SpawnManager->SpawnGroundLifeEssenceAtTransform(TargetTransform, 1);
				}
		}),
		i * 0.05f,
		false);
	}
}
