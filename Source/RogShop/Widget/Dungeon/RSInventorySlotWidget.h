// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSInventorySlotWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class ROGSHOP_API URSInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;


// 위젯 클릭
protected:
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
    void SetIsPressable(bool bNewIsPressable);

private:
    void HandleLongPress();

private:
    UPROPERTY()
    bool bIsPressable;  // 해당 값이 true인 경우 버튼 입력을 받습니다.

    float HoldThreshold;    // 마우스 좌클릭 키다운 시간

    FTimerHandle HoldTimerHandle;

// 현재 슬롯에 대한 아이템 정보
public:
    void SetSlotItemInfo(FName NewItemDataTableKey, UTexture2D* NewItemImage, FString NewItemCount);

    FName GetItemDataTableKey() const;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemInfo", meta = (BindWidget, AllowPrivateAccess = "true"))
    FName ItemDataTableKey;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemInfo", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<UImage> ItemIcon;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemInfo", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<UTextBlock> ItemCount;
};
