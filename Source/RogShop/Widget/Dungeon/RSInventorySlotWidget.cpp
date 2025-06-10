// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInventorySlotWidget.h"
#include "RogShop/UtilDefine.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

void URSInventorySlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnPressed.AddDynamic(this, &URSInventorySlotWidget::HandleSlotClicked);
    }

}

void URSInventorySlotWidget::SetSlotItemInfo(FName NewItemDataTableKey, UTexture2D* NewItemIcon, FString NewItemCount)
{
    if (NewItemDataTableKey != NAME_None)
    {
        ItemDataTableKey = NewItemDataTableKey;
    }

    if (ItemIcon)
    {
        ItemIcon->SetBrushFromTexture(NewItemIcon);
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

void URSInventorySlotWidget::HandleSlotClicked()
{
    OnSlotClicked.Broadcast(ItemDataTableKey);
}

UButton* URSInventorySlotWidget::GetSlotButton()
{
    return SlotButton;
}
