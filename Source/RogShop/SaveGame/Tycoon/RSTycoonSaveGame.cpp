// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonSaveGame.h"
#include "ItemSlot.h"
#include "RogShop/UtilDefine.h"

void URSTycoonSaveGame::AddIngredient(FName IngredientKey, int32 Amount)
{
	int32 NoneIndex = INDEX_NONE;
	for (int32 i = 0; i < Ingredients.Num(); i++)
	{
		if (Ingredients[i].ItemKey == IngredientKey)
		{
			Ingredients[i].Quantity += Amount;
			return;
		}

		if (Ingredients[i].ItemKey == FName() || Ingredients[i].ItemKey == TEXT(""))
		{
			NoneIndex = i;
		}
	}
	
	if (NoneIndex != INDEX_NONE)
	{
		Ingredients[NoneIndex].ItemKey = IngredientKey;
		Ingredients[NoneIndex].Quantity = Amount;
	}
	else
	{
		RS_LOG_C("재료를 추가하지 못했습니다.", FColor::Red);
	}
}
