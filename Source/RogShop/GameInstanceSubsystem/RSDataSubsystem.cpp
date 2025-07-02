// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDataSubsystem.h"

#include "RSDataSubsystemSettings.h"
#include "RogShop/UtilDefine.h"

void URSDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const URSDataSubsystemSettings* DataSettings = GetDefault<URSDataSubsystemSettings>();
	Food = DataSettings->FoodDataTable.LoadSynchronous();
	FoodDetail = DataSettings->FoodDetailDataTable.LoadSynchronous();
	IngredientInfo = DataSettings->IngredientDataTable.LoadSynchronous();
	IngredientDetail = DataSettings->IngredientDetailDataTable.LoadSynchronous();
	WeaponInfo = DataSettings->WeaponDataTable.LoadSynchronous();
	WeaponDetail = DataSettings->WeaponDetailDataTable.LoadSynchronous();
	RelicInfo = DataSettings->RelicDataTable.LoadSynchronous();
	RelicDetail = DataSettings->RelicDetailDataTable.LoadSynchronous();
	ForestMonsterSpawnGroup = DataSettings->ForestMonsterSpawnGroupDataTable.LoadSynchronous();
	DesertMonsterSpawnGroup = DataSettings->DesetMonsterSpawnGroupDataTable.LoadSynchronous();
	CaveMonsterSpawnGroup = DataSettings->CaveMonsterSpawnGroupDataTable.LoadSynchronous();
	Monster = DataSettings->MonsterDataTable.LoadSynchronous();
	DungeonLevel = DataSettings->DungeonLevelDataTable.LoadSynchronous();
	Guide = DataSettings->GuideDataTable.LoadSynchronous();
	GameFlowInfo = DataSettings->GameFlowInfoDataTable.LoadSynchronous();

	check(Food)
	check(FoodDetail)
	check(IngredientInfo)
	check(IngredientDetail)
	check(WeaponInfo)
	check(WeaponDetail)
	check(RelicInfo)
	check(RelicDetail)
	check(ForestMonsterSpawnGroup)
	check(DesertMonsterSpawnGroup)
	check(CaveMonsterSpawnGroup)
	check(Monster)
	check(DungeonLevel)
	check(Guide)
	check(GameFlowInfo)
}
