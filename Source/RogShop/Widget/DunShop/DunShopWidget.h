// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopItemStruct.h"
#include "DunShopWidget.generated.h"

class UHorizontalBox;

UCLASS()
class ROGSHOP_API UDunShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* ItemHorizontalBox;  // 아이템을 담을 가로 박스

    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<UUserWidget> ItemWidgetClass;  // 동적으로 생성할 아이템 위젯의 클래스

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
    UDataTable* ItemDataTable;

    void HandleItemPurchase(FName PurchasedID);

    // 희귀도 랜덤 추출 함수
    ERarity GetRandomRarity();

    // 데이터 테이블에서 확률 기반 아이템 랜덤 추출 함수
    FShopItemStruct* GetRandomItemFromDataTable(UDataTable* DataTable);

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
