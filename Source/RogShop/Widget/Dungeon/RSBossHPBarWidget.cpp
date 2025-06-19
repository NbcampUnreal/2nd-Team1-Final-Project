// Fill out your copyright notice in the Description page of Project Settings.

#include "RSBossHPBarWidget.h"
#include "Components/TextBlock.h"
#include "RSDunPlayerController.h"

void URSBossHPBarWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();

    if (RSDunPlayerController)
    {
        RSDunPlayerController->OnBossHPChange.AddDynamic(this, &URSBossHPBarWidget::UpdateBossHPBar);
    }
}

void URSBossHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

    SetVisibility(ESlateVisibility::Hidden);
}

void URSBossHPBarWidget::UpdateBossHPBar(FName BossName, float HPPercent)
{
    if (BossNameTextBlock)
    {
	    BossNameTextBlock->SetText(FText::FromName(BossName));
    }

    UpdateTargetPercent(HPPercent);

    if (HPPercent > 0)
    {
        SetVisibility(ESlateVisibility::HitTestInvisible);
    }
    else
    {
        SetVisibility(ESlateVisibility::Hidden);
    }
    
}
