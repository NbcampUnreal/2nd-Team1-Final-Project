// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInventorySlotImageWidget.h"

FReply URSInventorySlotImageWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // 좌클릭 키다운
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        GetWorld()->GetTimerManager().SetTimer(HoldTimerHandle, this, &URSInventorySlotImageWidget::HandleLongPress, HoldThreshold, false);
        return FReply::Handled();
    }

    return FReply::Unhandled();
}

FReply URSInventorySlotImageWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);
        return FReply::Handled();
    }

    return FReply::Unhandled();
}

void URSInventorySlotImageWidget::HandleLongPress()
{
    // 유물 슬롯이 아닐때만 처리
    if (SlotIndex != -1)
    {
        UE_LOG(LogTemp, Warning, TEXT("Long Press Detected on URSInventorySlotImageWidget Index: %d"), SlotIndex);

        // 추가 작업 필요
    }
}
