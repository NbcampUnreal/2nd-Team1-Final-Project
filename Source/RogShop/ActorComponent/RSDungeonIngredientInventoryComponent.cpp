// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonIngredientInventoryComponent.h"
#include "RogShop/UtilDefine.h"
#include "RSDungeonGroundIngredient.h"
#include "RSDunPlayerController.h"
#include "RSDunPlayerCharacter.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "ItemSlot.h"
#include "RSSaveGameSubsystem.h"
#include "RSDungeonIngredientSaveGame.h"
#include "Kismet/GameplayStatics.h"

void URSDungeonIngredientInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ARSDunPlayerCharacter* OwnerCharacter = GetOwner<ARSDunPlayerCharacter>();
	if (!OwnerCharacter)
	{
		return;
	}
	UGameInstance* CurGameInstance = OwnerCharacter->GetGameInstance();
	if (!CurGameInstance)
	{
		return;
	}

	URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
	if (!SaveGameSubsystem)
	{
		return;
	}

	SaveGameSubsystem->OnSaveRequested.AddDynamic(this, &URSDungeonIngredientInventoryComponent::SaveItemData);

	LoadItemData();
}

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
	int32 CurItemQuantity = GetQuantity(ItemKey);
	if (!CheckValidItem(ItemKey) || CurItemQuantity == 0)
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

	FItemInfoData* Data = CurCharacter->GetGameInstance()->GetSubsystem<URSDataSubsystem>()->IngredientInfo->FindRow<FItemInfoData>(ItemKey, TEXT("Get IngredientInfo"));

	int32 ItemIndex = -1;

	if (Data)
	{
		FText ItemName = Data->ItemName;
		UStaticMesh* ItemStaticMesh = Data->ItemStaticMesh;

		if (DungeonGroundItem && ItemStaticMesh)
		{
			DungeonGroundItem->InitGroundItemInfo(ItemName, false, ItemKey, ItemStaticMesh);
			DungeonGroundItem->SetQuantity(CurItemQuantity);
			DungeonGroundItem->RandImpulse();

			ItemIndex = RemoveItem(ItemKey, INT32_MAX);
		}
	}

	// UI 갱신되도록 이벤트 디스패처 호출
	ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(CurCharacter->GetController());
	if (!PC)
	{
		return;
	}
	if (ItemIndex == -1)
	{
		return;
	}

	PC->OnIngredientChange.Broadcast(ItemIndex, FItemSlot());
}

void URSDungeonIngredientInventoryComponent::SaveItemData()
{
	// SaveGame 오브젝트 생성
	URSDungeonIngredientSaveGame* IngredientSaveGame = Cast<URSDungeonIngredientSaveGame>(UGameplayStatics::CreateSaveGameObject(URSDungeonIngredientSaveGame::StaticClass()));
	if (!IngredientSaveGame)
	{
		return;
	}

	IngredientSaveGame->ItemList = ItemList;

	// 저장
	ARSDunPlayerCharacter* OwnerCharacter = GetOwner<ARSDunPlayerCharacter>();
	if (!OwnerCharacter)
	{
		return;
	}
	UGameInstance* CurGameInstance = OwnerCharacter->GetGameInstance();
	if (!CurGameInstance)
	{
		return;
	}

	URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
	if (!SaveGameSubsystem)
	{
		return;

	}

	UGameplayStatics::SaveGameToSlot(IngredientSaveGame, SaveGameSubsystem->IngredientInventorySaveSlotName, 0);
}

void URSDungeonIngredientInventoryComponent::LoadItemData()
{
	// 저장된 세이브 로드
	ARSDunPlayerCharacter* OwnerCharacter = GetOwner<ARSDunPlayerCharacter>();
	if (!OwnerCharacter)
	{
		return;
	}
	UGameInstance* CurGameInstance = OwnerCharacter->GetGameInstance();
	if (!CurGameInstance)
	{
		return;
	}

	URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
	if (!SaveGameSubsystem)
	{
		return;

	}

	URSDungeonIngredientSaveGame* IngredientLoadGame = Cast<URSDungeonIngredientSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSubsystem->IngredientInventorySaveSlotName, 0));
	if (IngredientLoadGame)
	{
		TArray<FItemSlot> LoadItemList = IngredientLoadGame->ItemList;
		for (size_t i = 0; i < LoadItemList.Num(); ++i)
		{
			AddItem(LoadItemList[i].ItemKey, LoadItemList[i].Quantity);
		}
	}
}
