// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonInventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/DataTable/CookFoodData.h"
#include "RogShop/GameInstanceSubsystem/RSDataSubsystem.h"
#include "RogShop/GameModeBase/RSTycoonGameModeBase.h"


URSTycoonInventoryComponent::URSTycoonInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URSTycoonInventoryComponent::AddItem(FName ItemKey, int32 Amount)
{
	if (GetFilledSize() >= GetSize())
	{
		RS_LOG_C("인벤토리가 가득 찼습니다", FColor::Red)
		return;
	}

	if (CheckValidItem(ItemKey))
	{
		ItemMap.Add(ItemKey, Amount);
	}
	else
	{
		RS_LOG_C("아이템 추가를 실패했습니다", FColor::Red);
	}
}

void URSTycoonInventoryComponent::RemoveItem(FName ItemKey, int32 Amount)
{
	if (CheckValidItem(ItemKey))
	{
		if (!ItemMap.Contains(ItemKey))
		{
			ItemMap.Add(ItemKey, 0);
		}

		ItemMap[ItemKey] -= Amount;

		if (ItemMap[ItemKey] < 0)
		{
			ItemMap.Remove(ItemKey);
		}
	}
	else
	{
		RS_LOG_C("아이템 삭제를 실패했습니다", FColor::Red);
	}
}

int32 URSTycoonInventoryComponent::GetAmount(const FName& ItemKey)
{
	return ItemMap.Contains(ItemKey) ? ItemMap[ItemKey] : INDEX_NONE;
}

void URSTycoonInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadItemData();

	// Widget = CreateWidget(GetWorld()->GetFirstPlayerController(), WidgetType);
}

void URSTycoonInventoryComponent::LoadItemData()
{
	//가지고 있는 음식 파일 로드
}

bool URSTycoonInventoryComponent::CheckValidItem(const FName& ItemKey)
{
	URSDataSubsystem* Data = GetWorld()->GetGameInstance()->GetSubsystem<URSDataSubsystem>();

	if (FCookFoodData* Row = Data->Food->FindRow<FCookFoodData>(ItemKey, TEXT("Contains FoodData")))
	{
		return true;
	}

	if (FIngredientData* Row = Data->Ingredient->FindRow<FIngredientData>(ItemKey, TEXT("Contains IngredientData")))
	{
		return true;
	}

	return false;
}

void URSTycoonInventoryComponent::SaveItemData()
{
	//가지고 있는 음식 파일 세이브
}
