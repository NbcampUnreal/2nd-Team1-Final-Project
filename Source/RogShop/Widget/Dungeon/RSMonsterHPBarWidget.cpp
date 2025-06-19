// Fill out your copyright notice in the Description page of Project Settings.

#include "RSMonsterHPBarWidget.h"
#include "Components/ProgressBar.h"

void URSMonsterHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TargetPercent = 1.0f;
	CurrentPercent = 1.0f;
	InterpSpeed = 5.0f;
}

void URSMonsterHPBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!MonsterHPBar || !MonsterHPBackBar) return;

	CurrentPercent = FMath::FInterpTo(CurrentPercent, TargetPercent, InDeltaTime, InterpSpeed);
	MonsterHPBackBar->SetPercent(CurrentPercent);

	// 데미지 감소시 색깔도 보간해서 좀 더 자연스럽게 구현
	FLinearColor StartColor = FLinearColor(1.0f, 0.953f, 0.8f);	// 흰색에 가까운 노란 잔상 색
	FLinearColor EndColor = FLinearColor(1.0f, 0.721f, 0.2f);	// 선명한 오렌지 빛 노란 색

	// 체력 차이가 작으면 흰색 → 크면 노랑
	float Diff = FMath::Abs(CurrentPercent - TargetPercent);
	float ColorLerpAlpha = FMath::Clamp(Diff * 10.0f, 0.0f, 1.0f);

	FLinearColor NewColor = FMath::Lerp(StartColor, EndColor, ColorLerpAlpha);
	MonsterHPBackBar->SetFillColorAndOpacity(NewColor);

}

void URSMonsterHPBarWidget::UpdateTargetPercent(const float NewPercent)
{
	TargetPercent = FMath::Clamp(NewPercent, 0.0f, 1.0f);  // 0 ~ 1 사이로 고정
	MonsterHPBar->SetPercent(TargetPercent);
}