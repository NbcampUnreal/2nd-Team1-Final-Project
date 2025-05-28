// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSPlayerInventoryWidget.generated.h"

class URSInventoryWeaponSlotWidget;
class URSInventoryRelicSlotWidget;
class URSInventoryIngredientSlotWidget;
class UButton;

UCLASS()
class ROGSHOP_API URSPlayerInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	URSInventoryWeaponSlotWidget* WeaponSlotWidget;

	UPROPERTY(meta = (BindWidget))
	URSInventoryRelicSlotWidget* RelicSlotWidget;

	UPROPERTY(meta = (BindWidget))
	URSInventoryIngredientSlotWidget* IngredientSlotWidget;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnExitBtnClicked();

	void SetMouseMode(bool bEnable);

	UPROPERTY(meta = (BindWidget))
	UButton* ExitBtn;
};
