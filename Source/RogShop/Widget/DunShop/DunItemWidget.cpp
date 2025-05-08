// Fill out your copyright notice in the Description page of Project Settings.
#include "DunItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

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
