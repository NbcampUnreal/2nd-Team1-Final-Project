// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSInventorySlotImageWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSInventorySlotImageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void SetSlotIndex(int32 InIndex) { SlotIndex = InIndex; }
    int32 GetSlotIndex() const { return SlotIndex; }

protected:
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    FTimerHandle HoldTimerHandle;

    // 마우스 좌클릭 키다운 시간
    float HoldThreshold = 0.5f;

    void HandleLongPress();

    // 슬롯 기본값, 유물 슬롯은 이 값을 할당받지 않아서 유물 및 재료 슬롯 구분을 위해서 사용
    UPROPERTY()
    int32 SlotIndex = -1;
};
