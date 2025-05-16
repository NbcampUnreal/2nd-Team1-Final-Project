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

	if (Food != nullptr)
	{
		RS_LOG("Food not Load")
	}
	
	if (Ingredient != nullptr)
	{
		RS_LOG("Ingredient not Load")
	}
	
	check(Food)
	check(Ingredient)
}
