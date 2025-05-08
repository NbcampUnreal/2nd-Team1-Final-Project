// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

    void SetMouseMode(bool bEnable);

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* ExitBtn;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GoldText;

    UFUNCTION()
    void OnExitClicked();
};
