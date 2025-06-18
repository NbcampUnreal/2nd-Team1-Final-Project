// Fill out your copyright notice in the Description page of Project Settings.

#include "RSMonsterHPBarWidget.h"
#include "Components/ProgressBar.h"

void URSMonsterHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TargetPercent = 1.0f;
	CurrentPercent = 1.0f;
	InterpSpeed = 3.0f;
}

void URSMonsterHPBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!MonsterHPBar) return;

	CurrentPercent = FMath::FInterpTo(CurrentPercent, TargetPercent, InDeltaTime, InterpSpeed);
	MonsterHPBar->SetPercent(CurrentPercent);
}

void URSMonsterHPBarWidget::UpdateTargetPercent(const float NewPercent)
{
	TargetPercent = FMath::Clamp(NewPercent, 0.0f, 1.0f);  // 0 ~ 1 사이로 고정
}