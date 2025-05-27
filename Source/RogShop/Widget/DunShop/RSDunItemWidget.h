// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopItemData.h"
#include "RSDunItemWidget.generated.h"

class URSDunShopWidget;

UCLASS()
class ROGSHOP_API URSDunItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 부모 위젯에서 넣어주는 값
    void SetItemData(const FShopItemData& InItemData);
    void SetParentShop(URSDunShopWidget* InShop);
    void SetItemRowName(FName RowName);

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnBuyClicked();

    bool BuyItem();

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
    URSDunShopWidget* ParentShop;

    UPROPERTY(EditDefaultsOnly)
    FShopItemData ItemData;

    UPROPERTY()
    FName CurrentRowName;
};
