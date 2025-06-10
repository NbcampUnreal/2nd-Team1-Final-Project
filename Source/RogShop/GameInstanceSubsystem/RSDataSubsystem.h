// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RSDataSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
public:
	UPROPERTY()
	TObjectPtr<UDataTable> Food;
	
	UPROPERTY()
	TObjectPtr<UDataTable> IngredientInfo;

	UPROPERTY()
	TObjectPtr<UDataTable> IngredientDetail;

	UPROPERTY()
	TObjectPtr<UDataTable> WeaponInfo;

	UPROPERTY()
	TObjectPtr<UDataTable> WeaponDetail;

	UPROPERTY()
	TObjectPtr<UDataTable> RelicInfo;

	UPROPERTY()
	TObjectPtr<UDataTable> RelicDetail;

	UPROPERTY()
	TObjectPtr<UDataTable> ForestMonsterSpawnGroup;

	UPROPERTY()
	TObjectPtr<UDataTable> DesertMonsterSpawnGroup;

	UPROPERTY()
	TObjectPtr<UDataTable> CaveMonsterSpawnGroup;

	UPROPERTY()
	TObjectPtr<UDataTable> Monster;

	UPROPERTY()
	TObjectPtr<UDataTable> DungeonLevel;

	UPROPERTY()
	TObjectPtr<UDataTable> DungeonObject;
};
