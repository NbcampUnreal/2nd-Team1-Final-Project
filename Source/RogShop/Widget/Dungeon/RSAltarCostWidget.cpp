// Fill out your copyright notice in the Description page of Project Settings.


#include "RSAltarCostWidget.h"
#include "Components/TextBlock.h"

void URSAltarCostWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void URSAltarCostWidget::ChangeColor(FLinearColor TargetColor)
{
	if (CostText)
	{
		CostText->SetColorAndOpacity(FSlateColor(TargetColor));
	}	
}

void URSAltarCostWidget::UpdateCost(FString NewCost)
{
	if (CostText)
	{
		CostText->SetText(FText::FromString(NewCost));
	}
}
