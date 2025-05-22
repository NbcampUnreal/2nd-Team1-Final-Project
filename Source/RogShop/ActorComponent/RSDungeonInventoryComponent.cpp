// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonInventoryComponent.h"
#include "RogShop/UtilDefine.h"
#include "RSDungeonGroundItem.h"
#include "GameFramework/Character.h"
#include "RSDataSubsystem.h"
#include "DungeonItemData.h"

void URSDungeonInventoryComponent::DropItem(FName ItemKey)
{
	// TODO : 인벤토리에 있는 아이템이 땅에 버려지도록 구현해야한다.

	if (CheckValidItem(ItemKey))
	{
		if (ItemMap.Contains(ItemKey))
		{
			ACharacter* CurCharacter = GetOwner<ACharacter>();
			if (CurCharacter)
			{
				ARSDungeonGroundItem* DungeonGroundItem = GetWorld()->SpawnActor<ARSDungeonGroundItem>(ARSDungeonGroundItem::StaticClass(), CurCharacter->GetActorTransform());

				FDungeonItemData* Data = CurCharacter->GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Ingredient->FindRow<FDungeonItemData>(ItemKey, TEXT("Get Ingredient"));

				if (Data)
				{
					UStaticMesh* ItemStaticMesh = Data->ItemStaticMesh;

					if (DungeonGroundItem && ItemStaticMesh)
					{
						DungeonGroundItem->InitItemInfo(ItemKey, ItemStaticMesh);

						RemoveItem(ItemKey, ItemMap[ItemKey]);

						return;
					}
				}
			}
		}
	}

	RS_LOG_C("아이템 드랍에 실패했습니다.", FColor::Red);
}
