// Fill out your copyright notice in the Description page of Project Settings.


#include "RSGameInstance.h"
#include "RogShop/UtilDefine.h"
#include "RSDataSubsystem.h"

void URSGameInstance::Init()
{
    Super::Init();

    PopulateShopItemsOneTime();
}

bool URSGameInstance::GetDebugLogEnabled() const
{
	return bEnableDebugLog;
}

void URSGameInstance::SetDebugLogEnabled(bool bEnable)
{
	bEnableDebugLog = bEnable;
}

const TArray<TPair<FName, FItemInfoData*>>& URSGameInstance::GetCachedDunShopItems() const
{
    return CachedDunShopItems;
}

void URSGameInstance::PopulateShopItemsOneTime()
{
    URSDataSubsystem* DataSubsystem = GetSubsystem<URSDataSubsystem>();

    if (!DataSubsystem || !DataSubsystem->WeaponInfo || !DataSubsystem->RelicInfo)
    {
        return;
    }

    TArray<TPair<FName, FItemInfoData*>> Result;
    TSet<FName> AlreadySpawnedIDs;

    // 원하는 아이템 출력 개수
    const int32 DesiredCount = 5;

    int32 WeaponRowCount = DataSubsystem->WeaponInfo ? DataSubsystem->WeaponInfo->GetRowNames().Num() : 0;
    int32 RelicRowCount = DataSubsystem->RelicInfo ? DataSubsystem->RelicInfo->GetRowNames().Num() : 0;

    int32 TotalRowCount = WeaponRowCount + RelicRowCount;

    // 원하는 아이템 출력 개수가 데이터 테이블의 모든 아이템을 합친 개수보다 많은 경우의 무한 루프 방지
    int32 MaxCount = FMath::Min(DesiredCount, TotalRowCount);
    
    while (Result.Num() < MaxCount)
    {
        TPair<FName, FItemInfoData*> RandomItem = GetRandomItemFromDataTable(DataSubsystem->WeaponInfo, DataSubsystem->RelicInfo);
        FName RowName = RandomItem.Key;
        FItemInfoData* ItemData = RandomItem.Value;

        if (!ItemData || AlreadySpawnedIDs.Contains(RowName) || PurchasedItemIDs.Contains(RowName))
        {
            continue;
        }

        AlreadySpawnedIDs.Add(RowName);
        Result.Add(RandomItem);
    }

    CachedDunShopItems = Result;
}

TPair<FName, struct FItemInfoData*> URSGameInstance::GetRandomItemFromDataTable(UDataTable* WeaponDataTable, UDataTable* RelicDataTable)
{
    if (!WeaponDataTable || !RelicDataTable)
    {
        return TPair<FName, FItemInfoData*>(FName("Invalid"), nullptr);
    }

    // 추출할 아이템 등급 선정
    EItemRarity SelectedRarity = GetRandomRarity();
    TArray<TPair<FName, FItemInfoData*>> AllItems;

    // 무기 데이터 테이블 값 추출
    const TMap<FName, uint8*>& WeaponMapRaw = WeaponDataTable->GetRowMap();
    for (const auto& Elem : WeaponMapRaw)
    {
        FItemInfoData* ItemData = reinterpret_cast<FItemInfoData*>(Elem.Value);
        if (ItemData && ItemData->ItemRarity == SelectedRarity)
        {
            AllItems.Add({ Elem.Key, ItemData });
        }
    }

    // 유물 데이터 테이블 값 추출
    const TMap<FName, uint8*>& RelicMapRaw = RelicDataTable->GetRowMap();
    for (const auto& Elem : RelicMapRaw)
    {
        FItemInfoData* ItemData = reinterpret_cast<FItemInfoData*>(Elem.Value);
        if (ItemData && ItemData->ItemRarity == SelectedRarity)
        {
            AllItems.Add({ Elem.Key, ItemData });
        }
    }

    // 뽑힌 아이템 중에서 동일 확률로 1개 반환
    if (AllItems.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, AllItems.Num() - 1);
        return AllItems[RandomIndex];
    }

    return TPair<FName, FItemInfoData*>(FName("Invalid"), nullptr);
}

EItemRarity URSGameInstance::GetRandomRarity()
{
    int32 Roll = FMath::RandRange(1, 100); // 1 ~ 100 사이 정수

    if (Roll <= 60)
    {
        return EItemRarity::Common;
    }
    else if (Roll <= 85) // 60 + 25
    {
        return EItemRarity::Rare;
    }
    else if (Roll <= 95) // 85 + 10
    {
        return EItemRarity::Epic;
    }
    else
    {
        return EItemRarity::Legendary;
    }
}