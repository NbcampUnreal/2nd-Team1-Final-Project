// Fill out your copyright notice in the Description page of Project Settings.

#include "RSGuideWidget.h"
#include "RSDataSubsystem.h"
#include "RSGuideButtonWidget.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

void URSGuideWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

    if (CloseButton)
    {
        CloseButton->OnClicked.AddDynamic(this, &URSGuideWidget::OnClickCloseButton);
    }

    CreateButtons();
}

void URSGuideWidget::CreateButtons()
{
    URSDataSubsystem* DataSubsystem = GetGameInstance()->GetSubsystem<URSDataSubsystem>();

    if (!DataSubsystem || !DataSubsystem->Guide)
    {
        return;
    }

    if (!GuideScrollBox)
    {
        return;
    }

    GuideScrollBox->ClearChildren();

    TArray<FName> RowNames = DataSubsystem->Guide->GetRowNames();

    for (const FName& RowName : RowNames)
    {
        const FGuideData* RowData = DataSubsystem->Guide->FindRow<FGuideData>(RowName, TEXT("CreateButtons"));

        if (!RowData)
        {
            continue;
        }

        URSGuideButtonWidget* GuideButton = CreateWidget<URSGuideButtonWidget>(GetWorld(), GuideButtonClass);

        if (!GuideButton)
        {
            continue;
        }

        GuideButton->SetPadding(20);
        GuideButton->SetGuideButtonData(RowName, RowData->ButtonTitle);

        // 커스텀 버튼 델리게이트 처리
        GuideButton->OnGuideButtonClicked.AddDynamic(this, &URSGuideWidget::OnGuideButtonClicked);

        GuideScrollBox->AddChild(GuideButton);
    }
}

void URSGuideWidget::OnGuideButtonClicked(FName CategoryID)
{
    URSDataSubsystem* DataSubsystem = GetGameInstance()->GetSubsystem<URSDataSubsystem>();

    if (!DataSubsystem || !DataSubsystem->Guide)
    {
        return;
    }

    const FGuideData* GuideData = DataSubsystem->Guide->FindRow<FGuideData>(CategoryID, TEXT("OnGuideButtonClicked"));

    if (!GuideData)
    {
        return;
    }

    if (GuideText)
    {
        GuideText->SetText(GuideData->GuideDescription);
    }

    if (GuideImage)
    {
        GuideImage->SetBrushFromTexture(GuideData->GuideImage);
    }
}

void URSGuideWidget::OnClickCloseButton()
{
    RemoveFromParent();
}