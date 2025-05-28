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
	
protected:
	virtual void NativeConstruct() override;
};
