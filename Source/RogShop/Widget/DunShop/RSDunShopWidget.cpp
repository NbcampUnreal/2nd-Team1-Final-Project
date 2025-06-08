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
        RSDunPlayerController->OnLifeEssenceChange.AddDynamic(this, &URSDunShopWidget::UpdateLifeEssence);
    }
}

void URSDunShopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ExitBtn)
    {
        ExitBtn->OnClicked.AddDynamic(this, &URSDunShopWidget::OnExitClicked);
    }

    SetMouseMode(true);

    PopulateShopItems();
}

void URSDunShopWidget::UpdateLifeEssence(int NewLifeEssence)
{
    if (LifeEssenceText)
    {
        FString LifeEssenceString = FString::Printf(TEXT("%d"), NewLifeEssence); // 정수 형태로 변환
        LifeEssenceText->SetText(FText::FromString(LifeEssenceString));
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

TPair<FName, FItemInfoData*> URSDunShopWidget::GetRandomItemFromDataTable(UDataTable* WeaponDataTable, UDataTable* RelicDataTable)
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

void URSDunShopWidget::PopulateShopItems()
{
    URSDataSubsystem* DataSubsystem = GetGameInstance()->GetSubsystem<URSDataSubsystem>();

    if (!DataSubsystem || !DataSubsystem->Weapon || !DataSubsystem->Relic)
    {
        RS_LOG_DEBUG("DataSubsystem or DataTables are null");
        return;
    }

    TSet<FName> AlreadySpawnedIDs;
    const int32 AddItemCount = 5;

    for (int32 i = 0; i < AddItemCount; ++i)
    {
        TPair<FName, FItemInfoData*> RandomItem = GetRandomItemFromDataTable(DataSubsystem->Weapon, DataSubsystem->Relic);

        FName FoundRowName = RandomItem.Key;
        FItemInfoData* ItemData = RandomItem.Value;

        if (!ItemData)
        {
            RS_LOG_DEBUG("ItemData is Null");
            continue;
        }

        // 중복 랜덤 생성 방지
        if (AlreadySpawnedIDs.Contains(FoundRowName))
        {
            continue;
        }

        // 이미 구매한 아이템 생성 방지
        URSGameInstance* GI = Cast<URSGameInstance>(GetGameInstance());
        if (!GI || GI->PurchasedItemIDs.Contains(FoundRowName))
        {
            continue;
        }

        AlreadySpawnedIDs.Add(FoundRowName);

        if (RSDunShopItemWidgetClass && ItemHorizontalBox)
        {
            URSDunItemWidget* NewRSDunItemWidget = CreateWidget<URSDunItemWidget>(this, RSDunShopItemWidgetClass);

            if (NewRSDunItemWidget)
            {
                NewRSDunItemWidget->SetItemData(*ItemData);
                NewRSDunItemWidget->SetItemRowName(FoundRowName);

                ItemHorizontalBox->AddChild(NewRSDunItemWidget);
            }
        }
    }
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