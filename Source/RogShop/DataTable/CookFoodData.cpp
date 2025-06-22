// Fill out your copyright notice in the Description page of Project Settings.


#include "CookFoodData.h"

#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Food/RSBaseFood.h"
#include "ItemSlot.h"

bool FCookFoodData::CanMake(const TArray<FItemSlot>& Ingredients) const
{
	if (NeedIngredients.Num() == 0)
	{
		RS_LOG_F_C("%s의 조합법이 설정되어 있지 않습니다", FColor::Red, *Name);
		return false;
	}

	for (auto& NeedIngredient : NeedIngredients)
	{
		//재료칸 돌면서 만들 수 있는 재료가 있는지 확인
		bool bNoItem = true;
		for (const FItemSlot& e : Ingredients)
		{
			if (NeedIngredient.Key == e.ItemKey)
			{
				if (NeedIngredient.Value > e.Quantity)
				{
					return false;
				}
				
				bNoItem = false;
			}
		}

		//현재 있는 재료들 다 돌았는데 만들 재료가 없으면 false
		if (bNoItem)
		{
			return false;
		}
	}

	//재료가 있으며, 제작 가능하다는 뜻
	return true;
}
