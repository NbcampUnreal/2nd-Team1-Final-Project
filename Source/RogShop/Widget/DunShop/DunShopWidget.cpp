// Fill out your copyright notice in the Description page of Project Settings.


#include "DunShopWidget.h"
#include "DunItemWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void UDunShopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UE_LOG(LogTemp, Warning, TEXT("DunShopWidget Open !"));

    SetMouseMode(true);

    if (ExitBtn)
    {
        ExitBtn->OnClicked.AddDynamic(this, &UDunShopWidget::OnExitClicked);
    }

    PopulateShopItems();
}

void UDunShopWidget::OnExitClicked()
{
    SetMouseMode(false);

    RemoveFromParent();
}

void UDunShopWidget::SetMouseMode(bool bEnable)
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

void UDunShopWidget::PopulateShopItems()
{
    const int32 ItemCount = 5;

    for (int32 i = 0; i < ItemCount; ++i)
    {
        FShopItemStruct* RandomItem = GetRandomItemFromDataTable(ItemDataTable);

        if (RandomItem)
        {
            UE_LOG(LogTemp, Warning, TEXT("Selected Item: %s (%s)"),
                *RandomItem->ItemName.ToString(),
                *UEnum::GetValueAsString(RandomItem->Rarity));

            if (ItemWidgetClass && ItemHorizontalBox)
            {
                // 위젯 생성
                UDunItemWidget* NewItemWidget = CreateWidget<UDunItemWidget>(this, ItemWidgetClass);

                if (NewItemWidget)
                {
                    // 아이템 정보를 설정
                    NewItemWidget->SetItemName(RandomItem->ItemName);
                    NewItemWidget->SetItemDescription(RandomItem->Description);
                    NewItemWidget->SetItemPrice(RandomItem->Price);
                    NewItemWidget->SetItemIcon(RandomItem->Icon);

                    // 위젯을 수평 박스에 추가
                    UHorizontalBoxSlot* BoxSlot = Cast<UHorizontalBoxSlot>(ItemHorizontalBox->AddChild(NewItemWidget));

                    if (BoxSlot)
                    {
                        BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
                    }
                }
            }

        }
    }
}

ERarity UDunShopWidget::GetRandomRarity()
{
    int32 Roll = FMath::RandRange(1, 100); // 1 ~ 100 사이 정수

    if (Roll <= 60)
        return ERarity::Common;
    else if (Roll <= 85) // 60 + 25
        return ERarity::Rare;
    else if (Roll <= 95) // 85 + 10
        return ERarity::Epic;
    else
        return ERarity::Legendary;
}

FShopItemStruct* UDunShopWidget::GetRandomItemFromDataTable(UDataTable* DataTable)
{
    if (!DataTable) return nullptr;

    // 1. 등급을 뽑는다
    ERarity SelectedRarity = GetRandomRarity();

    // 2. 전체 데이터 테이블 가져오기
    TArray<FShopItemStruct*> AllItems;
    DataTable->GetAllRows(TEXT("Context"), AllItems);

    // 3. 뽑은 등급과 일치하는 아이템들 필터링
    TArray<FShopItemStruct*> FilteredItems;
    for (FShopItemStruct* Item : AllItems)
    {
        if (Item && Item->Rarity == SelectedRarity) // Rarity는 Struct에 추가돼 있어야 함
        {
            FilteredItems.Add(Item);
        }
    }

    // 4. 필터링된 리스트에서 하나 랜덤 선택
    if (FilteredItems.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, FilteredItems.Num() - 1);
        return FilteredItems[RandomIndex];
    }

    return nullptr; // 선택된 등급에 해당하는 아이템이 없을 경우
}
