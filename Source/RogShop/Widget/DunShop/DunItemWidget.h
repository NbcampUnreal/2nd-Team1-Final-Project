// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DunItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API UDunItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    void SetItemName(FText ItemName);
    void SetItemDescription(FText ItemDescription);
    void SetItemPrice(int32 ItemPrice);
    void SetItemIcon(UTexture2D* ItemIcon);

protected:
    UPROPERTY(meta = (BindWidget))
    class UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemNameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemDesText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemPriceText;

    UPROPERTY(meta = (BindWidget))
    class UButton* BuyBtn;

    UFUNCTION()
    void OnBuyClicked();
};
