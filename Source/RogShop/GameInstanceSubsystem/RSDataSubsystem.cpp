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
	Relic = DataSettings->RelicDataTable.LoadSynchronous();
	ForestMonsterSpawnGroup = DataSettings->ForestMonsterSpawnGroupDataTable.LoadSynchronous();
	MonsterStateGroup = DataSettings->MonsterDataTable.LoadSynchronous();

	check(Food)
	check(Ingredient)
	check(Weapon)
	check(Relic)
	check(ForestMonsterSpawnGroup)
	check(MonsterStateGroup)
}
