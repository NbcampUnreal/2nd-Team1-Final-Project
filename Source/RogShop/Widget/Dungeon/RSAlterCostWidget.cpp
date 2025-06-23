// Fill out your copyright notice in the Description page of Project Settings.


#include "RSAlterCostWidget.h"
#include "Components/TextBlock.h"

void URSAlterCostWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void URSAlterCostWidget::ChangeColor(FLinearColor TargetColor)
{
	CostText->SetColorAndOpacity(FSlateColor(TargetColor));
}

void URSAlterCostWidget::UpdateCost(int32 NewCost)
{
	if (CostText)
	{
		CostText->SetText(FText::FromString(FString::FromInt(NewCost) + "%"));
	}
}
