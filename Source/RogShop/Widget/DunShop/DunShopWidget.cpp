// Fill out your copyright notice in the Description page of Project Settings.


#include "DunShopWidget.h"
#include "DunItemWidget.h"
#include "RSGameInstance.h"

#include "RSDunPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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

void UDunShopWidget::HandleItemPurchase(FName PurchasedID)
{
    URSGameInstance* GI = Cast<URSGameInstance>(GetGameInstance());

    if (GI)
    {
        GI->PurchasedItemIDs.Add(PurchasedID);  // 아이템 구매 후 아이디 추가
        UE_LOG(LogTemp, Warning, TEXT("Item Purchased: %s"), *PurchasedID.ToString());
    }
}

void UDunShopWidget::PopulateShopItems()
{
    TSet<FName> AlreadySpawnedIDs;

    const int32 ItemCount = 5;

    for (int32 i = 0; i < ItemCount; ++i)
    {
        FShopItemData* RandomItem = GetRandomItemFromDataTable(ItemDataTable);

        if (RandomItem)
        {
            // 중복 랜덤 생성 방지
            if (!RandomItem || AlreadySpawnedIDs.Contains(RandomItem->ItemID))
            {
                continue;
            }
            // 이미 샀던 아이템 생성 방지
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

                    // 위젯을 수평 박스에 추가
                    ItemHorizontalBox->AddChild(NewItemWidget);

                    // 위젯이 전부 동일한 크기로 설정
                    /*UHorizontalBoxSlot* BoxSlot = Cast<UHorizontalBoxSlot>(ItemHorizontalBox->AddChild(NewItemWidget));

                    if (BoxSlot)
                    {
                        BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
                    }*/
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

FShopItemData* UDunShopWidget::GetRandomItemFromDataTable(UDataTable* DataTable)
{
    if (!DataTable) return nullptr;

    // 1. 등급을 뽑는다
    ERarity SelectedRarity = GetRandomRarity();

    // 2. 전체 데이터 테이블 가져오기
    TArray<FShopItemData*> AllItems;
    DataTable->GetAllRows(TEXT("Context"), AllItems);

    // 3. 뽑은 등급과 일치하는 아이템들 필터링
    TArray<FShopItemData*> FilteredItems;
    for (FShopItemData* Item : AllItems)
    {
        if (Item && Item->Rarity == SelectedRarity)
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

    return nullptr;
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