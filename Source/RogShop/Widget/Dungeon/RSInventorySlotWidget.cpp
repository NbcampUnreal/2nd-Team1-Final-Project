// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInventorySlotWidget.h"
#include "RogShop/UtilDefine.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "RSDunPlayerCharacter.h"
#include "RSDungeonIngredientInventoryComponent.h"

void URSInventorySlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    bIsPressable = false;
    HoldThreshold = 0.5f;
}

FReply URSInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // 좌클릭 키다운
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        GetWorld()->GetTimerManager().SetTimer(HoldTimerHandle, this, &URSInventorySlotWidget::HandleLongPress, HoldThreshold, false);
        return FReply::Handled();
    }

    return FReply::Unhandled();
}

FReply URSInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);
        return FReply::Handled();
    }

    return FReply::Unhandled();
}

void URSInventorySlotWidget::HandleLongPress()
{
    // 유물 슬롯이 아닐때만 처리
    if (bIsPressable)
    {
        // 추가 작업 필요
        ARSDunPlayerCharacter* CurCharacter = GetOwningPlayerPawn<ARSDunPlayerCharacter>();
        if (!CurCharacter)
        {
            return;
        }

        URSDungeonIngredientInventoryComponent* IngredientInventoryComp = CurCharacter->GetRSDungeonIngredientInventoryComponent();
        if (!IngredientInventoryComp)
        {
            return;
        }

        IngredientInventoryComp->DropItem(ItemDataTableKey);

        RS_LOG("UI 클릭");
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
