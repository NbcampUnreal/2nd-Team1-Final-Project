// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopItemStruct.h"
#include "DunItemWidget.generated.h"

class UDunShopWidget;

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
    void SetItemType(EItemList InItemList);
    void SetItemRarity(ERarity InItemRarity);
    void SetItemID(FName InItemID);

    void SetParentShop(UDunShopWidget* InShop);

    void BuyItem();

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
    EItemList ItemListType;

    UPROPERTY()
    ERarity ItemRarity;

    UPROPERTY()
    FName ItemID;

    UPROPERTY()
    UDunShopWidget* ParentShop;

    UFUNCTION()
    void OnBuyClicked();
};
