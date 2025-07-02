// Fill out your copyright notice in the Description page of Project Settings.

#include "RSGuideButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void URSGuideButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GuideButton)
	{
		GuideButton->OnClicked.AddDynamic(this, &URSGuideButtonWidget::HandleButtonClicked);
	}
}

void URSGuideButtonWidget::SetGuideButtonData(FName InGuideID, const FText& InGuideButtonText)
{
	GuideID = InGuideID;

	if (GuideButtonText)
	{
		GuideButtonText->SetText(InGuideButtonText);
	}
}

void URSGuideButtonWidget::HandleButtonClicked()
{
	OnGuideButtonClicked.Broadcast(GuideID);
}
