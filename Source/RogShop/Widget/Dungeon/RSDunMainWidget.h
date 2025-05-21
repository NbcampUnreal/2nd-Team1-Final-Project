// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSDunMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSDunMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable)
    void UpdateWeaponImage(UTexture2D* NewWeaponImage);

    UFUNCTION(BlueprintCallable)
    void UpdateWeaponSlot(bool bIsFirst, UTexture2D* NewWeaponImage);

    UFUNCTION(BlueprintCallable)
    void UpdateHP();

    UFUNCTION(BlueprintCallable)
    void UpdateMaxHP();

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* LifeEssenceText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MaxHPText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HPText;

    UPROPERTY(meta = (BindWidget))
    class UImage* WeaponSlot1;

    UPROPERTY(meta = (BindWidget))
    class UImage* WeaponSlot2;
};
