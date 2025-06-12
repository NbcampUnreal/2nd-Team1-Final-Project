// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemInfoData.h"
#include "RSDunShopWidget.generated.h"

class UButton;
class UTextBlock;
class UHorizontalBox;

UCLASS()
class ROGSHOP_API URSDunShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION()
    void UpdateLifeEssence(int NewLifeEssence);

protected:
    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnExitClicked();

    void SetMouseMode(bool bEnable);
    void PopulateShopItems();

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> RSDunShopItemWidgetClass;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitBtn;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> LifeEssenceText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHorizontalBox> ItemHorizontalBox;
};
