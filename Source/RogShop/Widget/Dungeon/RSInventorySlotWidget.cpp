// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInventorySlotWidget.h"
#include "RogShop/UtilDefine.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

void URSInventorySlotWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (SlotButton)
    {
        SlotButton->OnPressed.AddDynamic(this, &URSInventorySlotWidget::HandleSlotClicked);
    }

}

void URSInventorySlotWidget::SetSlotItemInfo(FName NewItemDataTableKey, UObject* NewItemIcon, FString NewItemCount)
{
    ItemDataTableKey = NewItemDataTableKey;

    if (ItemIcon)
    {
        ItemIcon->SetBrushResourceObject(NewItemIcon);
        
        FLinearColor TargetColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
        
        if (!NewItemIcon)
        {
            TargetColor = FLinearColor(1.f, 1.f, 1.f, 0.f);
        }

        ItemIcon->SetBrushTintColor(TargetColor);
    }

    if (ItemCount)
    {
        ItemCount->SetText(FText::FromString(NewItemCount));
    }
}

FName URSInventorySlotWidget::GetItemDataTableKey() const
{
    return ItemDataTableKey;
}

UObject* URSInventorySlotWidget::GetItemIcon() const
{
    UObject* IconResourceObject = nullptr;

    if (ItemIcon)
    {
        const FSlateBrush& ItemIconBrush = ItemIcon->GetBrush();
        IconResourceObject = ItemIconBrush.GetResourceObject();
    }

    return IconResourceObject;
}

FString URSInventorySlotWidget::GetItemQuantity() const
{
    FString ItemCountString;

    if (ItemCount)
    {
        ItemCountString = ItemCount->GetText().ToString();
    }

    return ItemCountString;
}

void URSInventorySlotWidget::HandleSlotClicked()
{
    OnSlotClicked.Broadcast(ItemDataTableKey);
}

UButton* URSInventorySlotWidget::GetSlotButton()
{
    return SlotButton;
}
