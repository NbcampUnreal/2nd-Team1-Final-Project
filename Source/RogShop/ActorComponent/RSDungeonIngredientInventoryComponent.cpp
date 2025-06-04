// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonIngredientInventoryComponent.h"
#include "RogShop/UtilDefine.h"
#include "RSDungeonGroundIngredient.h"
#include "RSDunPlayerController.h"
#include "GameFramework/Character.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "ItemSlot.h"

int32 URSDungeonIngredientInventoryComponent::AddItem(FName ItemKey, int32 Amount)
{
	int32 ItemIndex = Super::AddItem(ItemKey, Amount);

	if (ItemIndex < 0)
	{
		RS_LOG_C("아이템 슬롯의 인덱스가 유효하지 않습니다.", FColor::Red);
		return -1;
	}

	ACharacter* CurCharacter = GetOwner<ACharacter>();
	if (!CurCharacter)
	{
		return -1;
	}

	// UI 갱신되도록 이벤트 디스패처 호출
	ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(CurCharacter->GetController());
	if (!PC)
	{
		return -1;
	}

	PC->OnIngredientChange.Broadcast(ItemIndex, ItemList[ItemIndex]);

	return ItemIndex;
}

int32 URSDungeonIngredientInventoryComponent::RemoveItem(FName ItemKey, int32 Amount)
{
	int32 ItemIndex = Super::RemoveItem(ItemKey, Amount);

	if (ItemIndex < 0)
	{
		RS_LOG_C("아이템 슬롯의 인덱스가 유효하지 않습니다.", FColor::Red);
		return -1;
	}

	ACharacter* CurCharacter = GetOwner<ACharacter>();
	if (!CurCharacter)
	{
		return -1;
	}

	// UI 갱신되도록 이벤트 디스패처 호출
	ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(CurCharacter->GetController());
	if (!PC)
	{
		return -1;
	}

	PC->OnIngredientChange.Broadcast(ItemIndex, ItemList[ItemIndex]);

	return ItemIndex;
}

void URSDungeonIngredientInventoryComponent::DropItem(FName ItemKey)
{
	if (!CheckValidItem(ItemKey) || GetQuantity(ItemKey) == 0)
	{
		RS_LOG_C("아이템 드랍에 실패했습니다.", FColor::Red);
		return;
	}

	ACharacter* CurCharacter = GetOwner<ACharacter>();
	UWorld* World = GetWorld();

	if (!CurCharacter || !World)
	{
		return;
	}

	ARSDungeonGroundIngredient* DungeonGroundItem = World->SpawnActor<ARSDungeonGroundIngredient>(ARSDungeonGroundIngredient::StaticClass(), CurCharacter->GetActorTransform());

	FItemInfoData* Data = CurCharacter->GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Ingredient->FindRow<FItemInfoData>(ItemKey, TEXT("Get Ingredient"));

	if (Data)
	{
		UStaticMesh* ItemStaticMesh = Data->ItemStaticMesh;

		if (DungeonGroundItem && ItemStaticMesh)
		{
			DungeonGroundItem->InitItemInfo(ItemKey, ItemStaticMesh);

			RemoveItem(ItemKey, INT32_MAX);
		}
	}
}
