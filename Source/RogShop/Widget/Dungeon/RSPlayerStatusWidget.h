// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSPlayerStatusWidget.generated.h"

class UTextBlock;
class URSInventorySlotWidget;

UCLASS()
class ROGSHOP_API URSPlayerStatusWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    virtual void NativeConstruct() override;

public:
    UFUNCTION()
    void UpdateWeaponSlot(int8 SlotIndex, FName WeaponKey);

    UFUNCTION()
    void UpdateHP();

    UFUNCTION()
    void UpdateMaxHP();

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<UTextBlock> MaxHPText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<UTextBlock> HPText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<URSInventorySlotWidget> WeaponSlot1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<URSInventorySlotWidget> WeaponSlot2;
};
