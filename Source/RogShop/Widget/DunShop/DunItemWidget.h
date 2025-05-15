// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopItemData.h"
#include "DunItemWidget.generated.h"

class UDunShopWidget;

UCLASS()
class ROGSHOP_API UDunItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    void SetItemData(const FShopItemData& InItemData);

    void SetParentShop(UDunShopWidget* InShop);

    bool BuyItem();

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

    UPROPERTY()
    UDunShopWidget* ParentShop;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FShopItemData ItemData;

    UFUNCTION()
    void OnBuyClicked();
};
