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

void UDunItemWidget::SetItemData(const FShopItemStruct& InItemData)
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
        // Character->SetPrice(ItemData.Price); // �÷��̾� ��� ���� ���� �Լ� �Ǵ� public ���� �ʿ�

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

    /*if (Character->GetPrice() - ItemData.Price < 0) // �÷��̾� ��� �������� ���� �Լ� �Ǵ� public ���� �ʿ�
    {
        UE_LOG(LogTemp, Warning, TEXT("BuyItem - More Gold Need"));
        return false;
    }*/

    bool bIsBuy = true;

    switch (ItemData.ItemList)
    {
        case EItemList::Potion:
        {
            float PotionValue = 0.0f;

            switch (ItemData.Rarity)
            {
                case ERarity::Common: PotionValue = 1.0f; break;
                case ERarity::Rare: PotionValue = 2.0f; break;
                case ERarity::Epic: PotionValue = 3.0f; break;
                case ERarity::Legendary: PotionValue = 4.0f; break;
            }

            // Character->AddHP(PotionValue);  // �÷��̾� HP ���� ���� �Լ� �Ǵ� public ���� �ʿ�

            break;
        }
        case EItemList::Sword:
        {
            // ���� 1 ...
            break;
        }
        case EItemList::BattleAxe:
        {
            // ���� 2 ...
            break;
        }
        case EItemList::Hammer:
        {
            // ���� 3 ...
            break;
        }
        default:
            UE_LOG(LogTemp, Warning, TEXT("Not Define Item"));
            bIsBuy = false;
            break;
    }

    return bIsBuy;
}

