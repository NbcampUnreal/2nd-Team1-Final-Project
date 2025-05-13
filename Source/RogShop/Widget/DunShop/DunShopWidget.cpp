// Fill out your copyright notice in the Description page of Project Settings.


#include "DunShopWidget.h"
#include "DunItemWidget.h"
#include "RSGameInstance.h"

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

void UDunShopWidget::HandleItemPurchase(FName PurchasedID)
{
    URSGameInstance* GI = Cast<URSGameInstance>(GetGameInstance());

    if (GI)
    {
        GI->PurchasedItemIDs.Add(PurchasedID);  // ������ ���� �� ���̵� �߰�
        UE_LOG(LogTemp, Warning, TEXT("Item Purchased: %s"), *PurchasedID.ToString());
    }
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
    TSet<FName> AlreadySpawnedIDs;

    const int32 ItemCount = 5;

    for (int32 i = 0; i < ItemCount; ++i)
    {
        FShopItemStruct* RandomItem = GetRandomItemFromDataTable(ItemDataTable);

        if (RandomItem)
        {
            // �ߺ� ���� ���� ����
            if (!RandomItem || AlreadySpawnedIDs.Contains(RandomItem->ItemID))
            {
                continue;
            }
            // �̹� ��� ������ ���� ����
            else
            {
                URSGameInstance* GI = Cast<URSGameInstance>(GetGameInstance());

                if (GI)
                {
                    if (GI->PurchasedItemIDs.Contains(RandomItem->ItemID))
                    {
                        continue;
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("GameInstance is not valid!"));
                }
            }

            AlreadySpawnedIDs.Add(RandomItem->ItemID);

            if (ItemWidgetClass && ItemHorizontalBox)
            {
                UDunItemWidget* NewItemWidget = CreateWidget<UDunItemWidget>(this, ItemWidgetClass);

                if (NewItemWidget)
                {
                    NewItemWidget->SetItemData(*RandomItem);
                    NewItemWidget->SetParentShop(this);

                    // ������ ���� �ڽ��� �߰�
                    UHorizontalBoxSlot* BoxSlot = Cast<UHorizontalBoxSlot>(ItemHorizontalBox->AddChild(NewItemWidget));

                    if (BoxSlot)
                    {
                        BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
                    }
                }
            }

        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ItemDataTable is Null"));
        }
    }
}

ERarity UDunShopWidget::GetRandomRarity()
{
    int32 Roll = FMath::RandRange(1, 100); // 1 ~ 100 ���� ����

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

    // 1. ����� �̴´�
    ERarity SelectedRarity = GetRandomRarity();

    // 2. ��ü ������ ���̺� ��������
    TArray<FShopItemStruct*> AllItems;
    DataTable->GetAllRows(TEXT("Context"), AllItems);

    // 3. ���� ��ް� ��ġ�ϴ� �����۵� ���͸�
    TArray<FShopItemStruct*> FilteredItems;
    for (FShopItemStruct* Item : AllItems)
    {
        if (Item && Item->Rarity == SelectedRarity)
        {
            FilteredItems.Add(Item);
        }
    }

    // 4. ���͸��� ����Ʈ���� �ϳ� ���� ����
    if (FilteredItems.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, FilteredItems.Num() - 1);
        return FilteredItems[RandomIndex];
    }

    return nullptr;
}
