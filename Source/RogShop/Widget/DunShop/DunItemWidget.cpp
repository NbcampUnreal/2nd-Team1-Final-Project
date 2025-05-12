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

void UDunItemWidget::OnBuyClicked()
{
    if (ItemPriceText)
    {
        ItemPriceText->SetText(FText::FromString("Sold Out"));
    }

    if (BuyBtn)
    {
        BuyBtn->SetIsEnabled(false);
    }

    BuyItem();
}

void UDunItemWidget::SetItemName(FText ItemName)
{
    if (ItemNameText)
    {
        ItemNameText->SetText(ItemName);
    }
}

void UDunItemWidget::SetItemDescription(FText ItemDescription)
{
    if (ItemDesText)
    {
        ItemDesText->SetText(ItemDescription);
    }
}

void UDunItemWidget::SetItemPrice(int32 ItemPrice)
{
    if (ItemPriceText)
    {
        ItemPriceText->SetText(FText::AsNumber(ItemPrice));
    }
}

void UDunItemWidget::SetItemIcon(UTexture2D* ItemIcon)
{
    if (ItemImage && ItemIcon)
    {
        ItemImage->SetBrushFromTexture(ItemIcon);
    }
}

void UDunItemWidget::SetItemType(EItemList InItemList)
{
    ItemListType = InItemList;
}

void UDunItemWidget::SetItemRarity(ERarity InItemRarity)
{
    ItemRarity = InItemRarity;
}

void UDunItemWidget::SetItemID(FName InItemID)
{
    ItemID = InItemID;
}

void UDunItemWidget::SetParentShop(UDunShopWidget* InShop)
{
    ParentShop = InShop;
}

void UDunItemWidget::BuyItem()
{
    ARSDunBaseCharacter* Character = Cast<ARSDunBaseCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (!Character) return;

    switch (ItemListType)
    {
        case EItemList::Potion:
        {
            float PotionValue = 0.0f;

            switch (ItemRarity)
            {
                case ERarity::Common: PotionValue = 1.0f; break;
                case ERarity::Rare: PotionValue = 2.0f; break;
                case ERarity::Epic: PotionValue = 3.0f; break;
                case ERarity::Legendary: PotionValue = 4.0f; break;
            }

            Character->AddHP(PotionValue);  // 캐릭터 클래스에 이 함수 구현 필요
            break;
        }
        case EItemList::Sword:
        {
            // ...
            break;
        }
        default:
            UE_LOG(LogTemp, Warning, TEXT("Not Define Item."));
            break;
    }

    if (ParentShop)
    {
        ParentShop->HandleItemPurchase(ItemID);
    }
    else 
    {
        UE_LOG(LogTemp, Warning, TEXT("ParentShop is nullptr"));
    }
}

