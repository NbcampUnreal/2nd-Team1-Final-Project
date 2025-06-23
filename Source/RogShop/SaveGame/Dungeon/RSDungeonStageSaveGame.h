// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RSDungeonStageSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSDungeonStageSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Seed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> UnspawnedWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> UnspawnedRelics;
};
