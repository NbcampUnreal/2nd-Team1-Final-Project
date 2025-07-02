// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseInventoryComponent.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/GameInstanceSubsystem/RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "ItemSlot.h"

URSBaseInventoryComponent::URSBaseInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void URSBaseInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadItemData();
}

int32 URSBaseInventoryComponent::AddItem(FName ItemKey, int32 Amount)
{
	if (CheckValidItem(ItemKey))
	{
		// 인벤토리 순회
		int32 EmptySlot = INDEX_NONE;
		for (size_t i = 0; i < ItemList.Num(); ++i)
		{
			FItemSlot& ItemSlot = ItemList[i];

			// 인벤토리에 해당 아이템이 존재하는 경우 아이템 추가
			if (ItemSlot.ItemKey == ItemKey)
			{
				ItemSlot.Quantity += Amount;
				return i;
			}

			// 인벤토리에 해당 슬롯이 비어있는 경우 아이템 추가			
			if (ItemSlot.ItemKey == NAME_None)
			{
				EmptySlot = i;
				ItemList[EmptySlot].ItemKey = ItemKey;
				ItemList[EmptySlot].Quantity = Amount;
				return i;
			}
		}

		if (EmptySlot != INDEX_NONE)
		{
			ItemList[EmptySlot].ItemKey = ItemKey;
			ItemList[EmptySlot].Quantity = Amount;
			return EmptySlot;
		}
	}

	RS_LOG_C("아이템을 추가하지 못했습니다.", FColor::Red);
	return -1;
}

int32 URSBaseInventoryComponent::RemoveItem(FName ItemKey, int32 Amount)
{
	// 키가 유효한지 확인
	if (CheckValidItem(ItemKey))
	{
		// 인벤토리 순회
		for (size_t i = 0; i < ItemList.Num(); ++i)
		{
			FItemSlot& ItemSlot = ItemList[i];

			// 인벤토리에 들어있는 경우
			if (ItemSlot.ItemKey == ItemKey)
			{
				// 아이템 개수만큼 제거
				ItemSlot.Quantity -= Amount;

				// 개수가 0 이하인 경우 해당 인덱스의 값을 기본 값으로 변경
				if (ItemSlot.Quantity <= 0)
				{
					ItemSlot.ItemKey = NAME_None;
					ItemSlot.Quantity = 0;
				}

				return i;
			}
		}
	}

	RS_LOG_C("아이템을 삭제하지 못했습니다.", FColor::Red);
	return -1;
}

int32 URSBaseInventoryComponent::GetQuantity(const FName& ItemKey)
{
	int32 Quantity = 0;
	// 인벤토리 순회
	for (FItemSlot& ItemSlot : ItemList)
	{
		// 인벤토리에 들어있는 경우
		if (ItemSlot.ItemKey == ItemKey)
		{
			Quantity = ItemSlot.Quantity;
		}
	}

	return Quantity;
}

void URSBaseInventoryComponent::SaveItemData()
{
	// TODO : 가지고 있는 음식 파일 세이브
}

void URSBaseInventoryComponent::LoadItemData()
{
	// TODO : 가지고 있는 음식 파일 로드
}

void URSBaseInventoryComponent::SetMaxSlot(int32 Size)
{
	ItemList.SetNum(Size);
}

bool URSBaseInventoryComponent::CheckValidItem(const FName& ItemKey)
{
	URSDataSubsystem* Data = GetWorld()->GetGameInstance()->GetSubsystem<URSDataSubsystem>();

	if (FItemInfoData* Row = Data->IngredientInfo->FindRow<FItemInfoData>(ItemKey, TEXT("Contains IngredientDetailData")))
	{
		return true;
	}

	RS_LOG_C("Key가 유효하지 않습니다.", FColor::Red);
	return false;
}
