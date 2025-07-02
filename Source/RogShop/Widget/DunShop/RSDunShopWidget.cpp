// Fill out your copyright notice in the Description page of Project Settings.

#include "RSDunShopWidget.h"

#include "RSGameInstance.h"
#include "RSDunItemWidget.h"
#include "RSDataSubsystem.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Kismet/GameplayStatics.h"

#include "RSDunPlayerCharacter.h"
#include "RSDunPlayerController.h"
#include "RogShop/UtilDefine.h"

void URSDunShopWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();

    if (RSDunPlayerController)
    {
        RSDunPlayerController->OnLifeEssenceChange.RemoveDynamic(this, &URSDunShopWidget::UpdateLifeEssence);
        RSDunPlayerController->OnLifeEssenceChange.AddDynamic(this, &URSDunShopWidget::UpdateLifeEssence);
    }

    if (ExitBtn)
    {
        ExitBtn->OnClicked.RemoveDynamic(this, &URSDunShopWidget::OnExitClicked);
        ExitBtn->OnClicked.AddDynamic(this, &URSDunShopWidget::OnExitClicked);
    }

    PopulateShopItemsOneTime();
}

void URSDunShopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    SetMouseMode(true);

    // 상점을 열 때도 보유 재화값 갱신
    ARSDunPlayerCharacter* PlayerChar = Cast<ARSDunPlayerCharacter>(GetOwningPlayerPawn());

    if (PlayerChar)
    {
        UpdateLifeEssence(PlayerChar->GetLifeEssence());
    }
}

void URSDunShopWidget::UpdateLifeEssence(int NewLifeEssence)
{
    if (LifeEssenceText)
    {
        FText DisplayText = FText::AsNumber(NewLifeEssence);
        LifeEssenceText->SetText(DisplayText);
    }
}

void URSDunShopWidget::PopulateShopItemsOneTime()
{
    URSDataSubsystem* DataSubsystem = GetGameInstance()->GetSubsystem<URSDataSubsystem>();

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

        // 이미 구매한 아이템 생성 방지
        URSGameInstance* GI = Cast<URSGameInstance>(GetGameInstance());

        if (!GI || GI->PurchasedItemIDs.Contains(RowName))
        {
            continue;
        }

        if (!ItemData || AlreadySpawnedIDs.Contains(RowName))
        {
            continue;
        }

        AlreadySpawnedIDs.Add(RowName);
        Result.Add(RandomItem);
    }

    CachedDunShopItems = Result;

    ShowShopItems();
}

void URSDunShopWidget::ShowShopItems()
{
    for (const auto& ItemPair : CachedDunShopItems)
    {
        const FName RowName = ItemPair.Key;
        const FItemInfoData* ItemData = ItemPair.Value;

        if (!ItemData)
        {
            continue;
        }

        if (RSDunShopItemWidgetClass && ItemHorizontalBox)
        {
            URSDunItemWidget* NewItemWidget = CreateWidget<URSDunItemWidget>(this, RSDunShopItemWidgetClass);
            if (NewItemWidget)
            {
                NewItemWidget->SetItemData(*ItemData);
                NewItemWidget->SetItemRowName(RowName);

                ItemHorizontalBox->AddChild(NewItemWidget);
            }
        }
    }
}

EItemRarity URSDunShopWidget::GetRandomRarity()
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

TPair<FName, struct FItemInfoData*> URSDunShopWidget::GetRandomItemFromDataTable(UDataTable* WeaponDataTable, UDataTable* RelicDataTable)
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

void URSDunShopWidget::OnExitClicked()
{
    SetMouseMode(false);

    RemoveFromParent();
}

void URSDunShopWidget::SetMouseMode(bool bEnable)
{
    UWorld* World = GetWorld();

    if (World)
    {
        APlayerController* PlayerController = World->GetFirstPlayerController();

        if (PlayerController)
        {
            // 받고있던 입력 초기화
            PlayerController->FlushPressedKeys();

            if (bEnable)
            {
                FInputModeUIOnly InputMode;
                PlayerController->SetInputMode(InputMode);
                PlayerController->bShowMouseCursor = true;
            }
            else
            {
                FInputModeGameOnly InputMode;
                PlayerController->SetInputMode(InputMode);
                PlayerController->bShowMouseCursor = false;
            }
        }
    }
}