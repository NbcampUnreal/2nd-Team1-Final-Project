// Fill out your copyright notice in the Description page of Project Settings.
#include "DunItemWidget.h"
#include "RSDunBaseCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "DunShopWidget.h"

void UDunItemWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BuyBtn)
    {
        BuyBtn->OnClicked.AddDynamic(this, &UDunItemWidget::OnBuyClicked);
    }
}

void UDunItemWidget::SetItemData(const FShopItemData& InItemData)
{
    ItemData = InItemData;

    if (ItemNameText)
    {
        ItemNameText->SetText(InItemData.ItemName);
    }

    if (ItemDesText)
    {
        ItemDesText->SetText(InItemData.Description);
    }

    if (ItemPriceText)
    {
        ItemPriceText->SetText(FText::AsNumber(InItemData.Price));
    }

    if (ItemImage && InItemData.Icon)
    {
        ItemImage->SetBrushFromTexture(InItemData.Icon);
    }
}

void UDunItemWidget::SetParentShop(UDunShopWidget* InShop)
{
    ParentShop = InShop;
}

void UDunItemWidget::OnBuyClicked()
{
    if (BuyItem())
    {
        // Character->SetLifeEssence(ItemData.Price); // 플레이어 골드 수정 관련 함수 또는 public 변수 필요

        if (ItemPriceText)
        {
            ItemPriceText->SetText(FText::FromString("Sold Out"));
        }

        if (BuyBtn)
        {
            BuyBtn->SetIsEnabled(false);
        }

        if (ParentShop)
        {
            ParentShop->HandleItemPurchase(ItemData.ItemID);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ParentShop is nullptr"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Buy Item"));
    }
}

bool UDunItemWidget::BuyItem()
{
    ARSDunBaseCharacter* Character = Cast<ARSDunBaseCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("BuyItem - Character is nullptr"));
        return false;
    }

    /*if (Character->GetLifeEssence() - ItemData.Price < 0) // 플레이어 재화 가져오기 관련 함수 또는 public 변수 필요
    {
        UE_LOG(LogTemp, Warning, TEXT("BuyItem - More LifeEssence Need"));
        return false;
    }*/

    bool bIsBuy = true;

    float FinalValue = 0.0f;

    switch (ItemData.ItemList)
    {
        case EItemList::Potion:
        {
            switch (ItemData.Rarity)
            {
                case ERarity::Common: FinalValue = 1.0f; break;
                case ERarity::Rare: FinalValue = 2.0f; break;
                case ERarity::Epic: FinalValue = 3.0f; break;
                case ERarity::Legendary: FinalValue = 4.0f; break;
            }

            // Character->AddHP(FinalValue);  // 플레이어 HP 수정 관련 함수 또는 public 변수 필요

            break;
        }
        case EItemList::MaxHpRelic:
        {
            switch (ItemData.Rarity)
            {
                case ERarity::Common: FinalValue = 1.0f; break;
                case ERarity::Rare: FinalValue = 2.0f; break;
                case ERarity::Epic: FinalValue = 3.0f; break;
                case ERarity::Legendary: FinalValue = 4.0f; break;
            }

            // Character->SetMaxHP(FinalValue);

            break;
        }
        case EItemList::WalkSpeedRelic:
        {
            switch (ItemData.Rarity)
            {
                case ERarity::Common: FinalValue = 1.0f; break;
                case ERarity::Rare: FinalValue = 2.0f; break;
                case ERarity::Epic: FinalValue = 3.0f; break;
                case ERarity::Legendary: FinalValue = 4.0f; break;
            }

            // Character->AddWalkSpeed(FinalValue);

            break;
        }
        case EItemList::AttackRelic:
        {
            switch (ItemData.Rarity)
            {
                case ERarity::Common: FinalValue = 5.0f; break;
                case ERarity::Rare: FinalValue = 10.0f; break;
                case ERarity::Epic: FinalValue = 15.0f; break;
                case ERarity::Legendary: FinalValue = 25.0f; break;
            }

            // Character->AddAtk(FinalValue);

            break;
        }
        case EItemList::AttackSpeedRelic:
        {
            switch (ItemData.Rarity)
            {
                case ERarity::Common: FinalValue = 5.0f; break;
                case ERarity::Rare: FinalValue = 10.0f; break;
                case ERarity::Epic: FinalValue = 15.0f; break;
                case ERarity::Legendary: FinalValue = 25.0f; break;
            }

            // Character->AddAtkSpeed(FinalValue);

            break;
        }
        case EItemList::Sword:
        {
            // 무기 1 ...
            break;
        }
        case EItemList::BattleAxe:
        {
            // 무기 2 ...
            break;
        }
        case EItemList::Hammer:
        {
            // 무기 3 ...
            break;
        }
        default:
            UE_LOG(LogTemp, Warning, TEXT("Not Define Item"));
            bIsBuy = false;
            break;
    }

    return bIsBuy;
}

