// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDataSubsystem.h"

#include "RogShop/RSDataSubsystemSettings.h"
#include "RogShop/UtilDefine.h"

void URSDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const URSDataSubsystemSettings* DataSettings = GetDefault<URSDataSubsystemSettings>();
	Food = DataSettings->FoodDataTable.LoadSynchronous();
	IngredientInfo = DataSettings->IngredientDataTable.LoadSynchronous();
	IngredientDetail = DataSettings->IngredientDetailDataTable.LoadSynchronous();
	WeaponInfo = DataSettings->WeaponDataTable.LoadSynchronous();
	WeaponDetail = DataSettings->WeaponDetailDataTable.LoadSynchronous();
	RelicInfo = DataSettings->RelicDataTable.LoadSynchronous();
	RelicDetail = DataSettings->RelicDetailDataTable.LoadSynchronous();
	ForestMonsterSpawnGroup = DataSettings->ForestMonsterSpawnGroupDataTable.LoadSynchronous();
	Monster = DataSettings->MonsterDataTable.LoadSynchronous();
	DungeonLevel = DataSettings->DungeonLevelDataTable.LoadSynchronous();
	DungeonObject = DataSettings->DungeonObjectDataTable.LoadSynchronous();

	check(Food)
	check(IngredientInfo)
	check(WeaponInfo)
	check(WeaponDetail)
	check(RelicInfo)
	check(RelicDetail)
	check(ForestMonsterSpawnGroup)
	check(Monster)
}
