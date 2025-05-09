// Fill out your copyright notice in the Description page of Project Settings.
#include "DunItemWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

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