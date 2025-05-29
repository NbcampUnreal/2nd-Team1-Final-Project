// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSInventoryWeaponSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSInventoryWeaponSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void UpdateWeaponSlot(uint8 SlotIndex, FName WeaponKey);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	class UImage* WeaponSlot1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	class UImage* WeaponSlot2;
};
