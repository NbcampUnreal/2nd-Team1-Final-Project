// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ItemInfoData.h"
#include "RSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 로그 입출력 관리
	bool GetDebugLogEnabled() const;
	void SetDebugLogEnabled(bool bEnable);
	
	const TArray<TPair<FName, FItemInfoData*>>& GetCachedDunShopItems() const;

	// 구매한 아이템의 아이디 저장
	TArray<FName> PurchasedItemIDs;

private:
	virtual void Init() override;

	bool bEnableDebugLog = false;

	// 상점 아이템 한 번만 추출하는 함수
	void PopulateShopItemsOneTime();
	EItemRarity GetRandomRarity();
	TPair<FName, struct FItemInfoData*> GetRandomItemFromDataTable(UDataTable* WeaponDataTable, UDataTable* RelicDataTable);

	TArray<TPair<FName, FItemInfoData*>> CachedDunShopItems;
};
