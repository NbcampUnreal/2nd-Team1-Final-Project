// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDataSubsystem.h"

#include "RogShop/RSDataSubsystemSettings.h"
#include "RogShop/UtilDefine.h"

void URSDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const URSDataSubsystemSettings* DataSettings = GetDefault<URSDataSubsystemSettings>();
	Food = DataSettings->FoodDataTable.LoadSynchronous();
	Ingredient = DataSettings->IngredientDataTable.LoadSynchronous();
	Weapon = DataSettings->WeaponDataTable.LoadSynchronous();
	WeaponClass = DataSettings->WeaponClassDataTable.LoadSynchronous();
	Relic = DataSettings->RelicDataTable.LoadSynchronous();
	RelicClass = DataSettings->RelicClassDataTable.LoadSynchronous();
	ForestMonsterSpawnGroup = DataSettings->ForestMonsterSpawnGroupDataTable.LoadSynchronous();
	Monster = DataSettings->MonsterDataTable.LoadSynchronous();
	DungeonLevel = DataSettings->DungeonLevelDataTable.LoadSynchronous();
	DungeonObject = DataSettings->DungeonObjectDataTable.LoadSynchronous();

	check(Food)
	check(Ingredient)
	check(Weapon)
	check(WeaponClass)
	check(Relic)
	check(RelicClass)
	check(ForestMonsterSpawnGroup)
	check(Monster)
}
