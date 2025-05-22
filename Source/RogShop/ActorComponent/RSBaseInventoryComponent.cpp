// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseInventoryComponent.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/GameInstanceSubsystem/RSDataSubsystem.h"
#include "RogShop/DataTable/CookFoodData.h"

URSBaseInventoryComponent::URSBaseInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SlotMaxSize = 5;
}


void URSBaseInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadItemData();
	
}

void URSBaseInventoryComponent::AddItem(FName ItemKey, int32 Amount)
{
	if (GetFilledSize() >= GetSlotMaxSize() && !ItemMap.Contains(ItemKey))
	{
		RS_LOG_C("Inventory is full", FColor::Red)
		return;
	}

	if (CheckValidItem(ItemKey))
	{
		ItemMap.Add(ItemKey, Amount);
	}
	else
	{
		RS_LOG_C("Failed to add item", FColor::Red);
	}
}

void URSBaseInventoryComponent::RemoveItem(FName ItemKey, int32 Amount)
{
	// 키가 유효한지 확인
	if (CheckValidItem(ItemKey))
	{
		// 인벤토리에 들어있는 경우
		if (ItemMap.Contains(ItemKey))
		{
			// 아이템 개수만큼 제거
			ItemMap[ItemKey] -= Amount;

			// 개수가 0 이하인 경우 맵에서 데이터 제거
			if (ItemMap[ItemKey] <= 0)
			{
				ItemMap.Remove(ItemKey);
			}
		}
	}
	else
	{
		RS_LOG_C("Failed to remove item", FColor::Red);
	}
}

int32 URSBaseInventoryComponent::GetAmount(const FName& ItemKey)
{
	return ItemMap.Contains(ItemKey) ? ItemMap[ItemKey] : INDEX_NONE;
}

void URSBaseInventoryComponent::SaveItemData()
{
	// TODO : 가지고 있는 음식 파일 세이브
}

void URSBaseInventoryComponent::LoadItemData()
{
	// TODO : 가지고 있는 음식 파일 로드
}

bool URSBaseInventoryComponent::CheckValidItem(const FName& ItemKey)
{
	URSDataSubsystem* Data = GetWorld()->GetGameInstance()->GetSubsystem<URSDataSubsystem>();

	if (FIngredientData* Row = Data->Ingredient->FindRow<FIngredientData>(ItemKey, TEXT("Contains IngredientData")))
	{
		return true;
	}

	return false;
}
