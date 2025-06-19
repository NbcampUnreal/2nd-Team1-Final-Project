// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSPlayerFloatingDamageWidget.generated.h"

class UTextBlock;

UCLASS()
class ROGSHOP_API URSPlayerFloatingDamageWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION()
    void FloatingValue(float Value);

protected:
    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TextDamage;

    // 페이드 애니메이션
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<UWidgetAnimation> FadeAnim;

private:
    UFUNCTION()
    void OnFadeOutFinished();

    FTimerHandle HideTextTimerHandle;
};
