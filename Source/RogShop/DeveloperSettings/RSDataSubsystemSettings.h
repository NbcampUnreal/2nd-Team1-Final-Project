// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RSDataSubsystemSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class ROGSHOP_API URSDataSubsystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> FoodDataTable;
	
	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> FoodDetailDataTable;
	
	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> IngredientDataTable;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> IngredientDetailDataTable;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> WeaponDetailDataTable;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> RelicDataTable;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> RelicDetailDataTable;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> ForestMonsterSpawnGroupDataTable;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> DesetMonsterSpawnGroupDataTable;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> CaveMonsterSpawnGroupDataTable;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> MonsterDataTable;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> DungeonLevelDataTable;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> GuideDataTable;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UDataTable> GameFlowInfoDataTable;
};
