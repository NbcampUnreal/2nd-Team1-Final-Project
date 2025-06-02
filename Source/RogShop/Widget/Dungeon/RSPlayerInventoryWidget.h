// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSPlayerInventoryWidget.generated.h"

class URSWeaponInventoryWidget;
class URSRelicInventoryWidget;
class URSIngredientInventoryWidget;
class UButton;

UCLASS()
class ROGSHOP_API URSPlayerInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<URSWeaponInventoryWidget> WeaponInventoryWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<URSIngredientInventoryWidget> IngredientInventoryWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<URSRelicInventoryWidget> RelicInventoryWidget;

private:
	UFUNCTION()
	void OnExitBtnClicked();

	void SetMouseMode(bool bEnable);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitBtn;
};
