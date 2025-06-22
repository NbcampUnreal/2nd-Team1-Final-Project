// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemInfoData.h"
#include "RSDunItemWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;

UCLASS()
class ROGSHOP_API URSDunItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 부모 위젯에서 넣어주는 값
    void SetItemData(const FItemInfoData& InItemData);

    // 해당 아이템 키 값 세팅
    void SetItemRowName(FName RowName);

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnBuyClicked();

    bool BuyItem();

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> ItemImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ItemNameText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ItemDesText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ItemPriceText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> BuyBtn;

    UPROPERTY()
    FItemInfoData ItemData;

    UPROPERTY()
    FName CurrentRowName;
};
