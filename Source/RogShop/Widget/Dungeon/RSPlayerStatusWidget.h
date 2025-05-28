// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSPlayerStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSPlayerStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

public:
    UFUNCTION(BlueprintCallable)
    void UpdateWeaponSlot(uint8 SlotIndex, FName WeaponKey);

    UFUNCTION(BlueprintCallable)
    void UpdateHP();

    UFUNCTION(BlueprintCallable)
    void UpdateMaxHP();

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
    class UTextBlock* LifeEssenceText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
    class UTextBlock* MaxHPText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
    class UTextBlock* HPText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
    class UImage* WeaponSlot1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
    class UImage* WeaponSlot2;
};
