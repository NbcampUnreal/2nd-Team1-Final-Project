// Fill out your copyright notice in the Description page of Project Settings.


#include "CookFoodData.h"

#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Food/RSBaseFood.h"
#include "ItemSlot.h"

bool FCookFoodData::CanMake(const TArray<FItemSlot>& Ingredients) const
{
	if (NeedIngredients.Num() == 0)
	{
		RS_LOG_F_C("%s의 조합법이 설정되어 있지 않습니다", FColor::Red, *Name)
		return false;
	}
	
	for (auto& NeedIngredient : NeedIngredients)
	{
		FName FindIngredientName = NeedIngredient.Key;

		for (FItemSlot e : Ingredients)
		{
			if (NeedIngredient.Key == e.ItemKey && NeedIngredient.Value > e.Quantity)
			{
				return false;
			}
		}
	}

	return true;
}
