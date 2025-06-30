// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSInventorySlotWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, FName, SlotItemDataTableKey);

UCLASS()
class ROGSHOP_API URSInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeOnInitialized() override;

// 현재 슬롯에 대한 아이템 정보
public:
    void SetSlotItemInfo(FName NewItemDataTableKey, UObject* NewItemImage, FString NewItemCount);

    FName GetItemDataTableKey() const;

    UObject* GetItemIcon() const;

    FString GetItemQuantity() const;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemInfo", meta = (AllowPrivateAccess = "true"))
    FName ItemDataTableKey;

public:
    FOnSlotClicked OnSlotClicked;

private:
    UFUNCTION()
    void HandleSlotClicked();

// 위젯
public:
    UButton* GetSlotButton();

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemInfo", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<UImage> ItemIcon;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemInfo", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<UTextBlock> ItemCount;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemInfo", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<UButton> SlotButton;
};
