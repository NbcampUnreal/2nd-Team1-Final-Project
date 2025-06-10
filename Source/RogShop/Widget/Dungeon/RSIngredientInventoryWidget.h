// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSIngredientInventoryWidget.generated.h"

class UUniformGridPanel;
class URSInventorySlotWidget;

struct FItemSlot;

UCLASS()
class ROGSHOP_API URSIngredientInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
    void CreateSlots(int32 NumSlots, int32 NumColumns);

    // 슬롯 수, 열을 기준으로 슬롯 생성
    UFUNCTION()
    void UpdateSlots(int32 IngredientSlotIndex, FItemSlot IngredientItemSlot);

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<UUniformGridPanel> IngredientSlots;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<URSInventorySlotWidget> InvecntorySlotWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
    TArray<TObjectPtr<URSInventorySlotWidget>> InvecntorySlots;

// 슬롯 클릭시 재료 드랍
private:
    UFUNCTION()
    void IngredientSlotPress();

    UFUNCTION()
    void IngredientSlotRelease();

    UFUNCTION()
    void IngredientDrop();

    UFUNCTION()
    void SetClickIngredientName(FName NewClickIngredientName);

private:
    float HoldThreshold;    // 마우스 좌클릭 키다운 시간

    FTimerHandle HoldTimerHandle;   // 마우스 좌클릭 타이머

    FName ClickIngredientName;  // 클릭한 슬롯의 재료의 데이터 테이블 RowName
};
