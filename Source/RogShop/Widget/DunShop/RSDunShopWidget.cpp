// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunShopWidget.h"

#include "RSGameInstance.h"
#include "RSDunItemWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

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

void URSDunShopWidget::HandleItemPurchase(FName PurchasedID)
{
    URSGameInstance* GI = Cast<URSGameInstance>(GetGameInstance());

    if (GI)
    {
        GI->PurchasedItemIDs.Add(PurchasedID);  // 아이템 구매 후 아이디 추가
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

TPair<FName, FDungeonItemData*> URSDunShopWidget::GetRandomItemFromDataTable()
{
    // 추출할 아이템 등급 선정
    EItemRarity SelectedRarity = GetRandomRarity();
    TArray<TPair<FName, FDungeonItemData*>> AllItems;

    // 무기 데이터 테이블 값 추출
    const TMap<FName, uint8*>& WeaponMapRaw = WeaponDataTable->GetRowMap();
    for (const auto& Elem : WeaponMapRaw)
    {
        FDungeonItemData* ItemData = reinterpret_cast<FDungeonItemData*>(Elem.Value);
        if (ItemData && ItemData->ItemRarity == SelectedRarity)
        {
            AllItems.Add(TPair<FName, FDungeonItemData*>(Elem.Key, ItemData));
        }
    }

    // 유물 데이터 테이블 값 추출
    const TMap<FName, uint8*>& RelicMapRaw = RelicDataTable->GetRowMap();
    for (const auto& Elem : RelicMapRaw)
    {
        FDungeonItemData* ItemData = reinterpret_cast<FDungeonItemData*>(Elem.Value);
        if (ItemData && ItemData->ItemRarity == SelectedRarity)
        {
            AllItems.Add(TPair<FName, FDungeonItemData*>(Elem.Key, ItemData));
        }
    }

    // 뽑힌 아이템 중에서 동일 확률로 1개 반환
    if (AllItems.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, AllItems.Num() - 1);
        return AllItems[RandomIndex];
    }

    // 유효하지 않은 경우
    return TPair<FName, FDungeonItemData*>(FName("Invalid"), nullptr);
}

void URSDunShopWidget::OnExitClicked()
{
    SetMouseMode(false);

    // 걷는 캐릭터 정지 필요시
    //if (APlayerController* PC = GetOwningPlayer())
    //{
    //    if (ARSDunPlayerCharacter* Player = Cast<ARSDunPlayerCharacter>(PC->GetPawn()))
    //    {
    //        Player->GetCharacterMovement()->MaxWalkSpeed = 600.f;
    //    }
    //}

    RemoveFromParent();
}

void URSDunShopWidget::PopulateShopItems()
{
    if (WeaponDataTable && RelicDataTable)
    {
        TSet<FName> AlreadySpawnedIDs;

        const int32 AddItemCount = 5;

        for (int32 i = 0; i < AddItemCount; ++i)
        {
            TPair<FName, FDungeonItemData*> RandomItem = GetRandomItemFromDataTable();

            FName FoundRowName = RandomItem.Key;
            FDungeonItemData* ItemData = RandomItem.Value;

            // 유효성 검사
            if (!ItemData)
            {
                UE_LOG(LogTemp, Warning, TEXT("ItemData is Null"));
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
                    NewRSDunItemWidget->SetParentShop(this);
                    NewRSDunItemWidget->SetItemRowName(FoundRowName);

                    ItemHorizontalBox->AddChild(NewRSDunItemWidget);
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is Null"));
    }
}

void URSDunShopWidget::SetMouseMode(bool bEnable)
{
    UWorld* World = GetWorld();

    if (World)
    {
        APlayerController* PlayerController = World->GetFirstPlayerController();

        if (PlayerController)
        {
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