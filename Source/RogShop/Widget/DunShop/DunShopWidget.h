// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopItemData.h"
#include "DunShopWidget.generated.h"

class UHorizontalBox;

UCLASS()
class ROGSHOP_API UDunShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* ItemHorizontalBox;

    // BP �Ҵ� �ʿ�
    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<UUserWidget> ItemWidgetClass;

    // ������ ���̺� �Ҵ� �ʿ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
    UDataTable* ItemDataTable;

    void HandleItemPurchase(FName PurchasedID);

    // ��͵� ���� ���� �Լ�
    ERarity GetRandomRarity();

    // ������ ���̺��� Ȯ�� ��� ������ ���� ���� �Լ�
    FShopItemData* GetRandomItemFromDataTable(UDataTable* DataTable);

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* ExitBtn;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GoldText;

    UFUNCTION()
    void OnExitClicked();

    void SetMouseMode(bool bEnable);
    void PopulateShopItems();
};
