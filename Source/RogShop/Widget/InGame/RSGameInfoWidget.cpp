// Fill out your copyright notice in the Description page of Project Settings.

#include "RSGameInfoWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "RSDataSubsystem.h"
#include "GameFlowInfoData.h"

void URSGameInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &URSGameInfoWidget::OnClickConfirmButton);
	}

	if (GameInfoLeftButton)
	{
		GameInfoLeftButton->OnClicked.AddDynamic(this, &URSGameInfoWidget::HandleLeftButtonClicked);
	}

	if (GameInfoLeftButton)
	{
		GameInfoRightButton->OnClicked.AddDynamic(this, &URSGameInfoWidget::HandleRightButtonClicked);
	}

	DataSubsystem = GetGameInstance()->GetSubsystem<URSDataSubsystem>();

	if (DataSubsystem && DataSubsystem->GameFlowInfo)
	{
		PageRowNames = DataSubsystem->GameFlowInfo->GetRowNames();
		CurrentPageIndex = 0;
		UpdatePage(CurrentPageIndex);
	}
}

void URSGameInfoWidget::OnClickConfirmButton()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->SetShowMouseCursor(false);

		// InputMode 변경
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);

		PC->FlushPressedKeys();
	}

	RemoveFromParent();
}

void URSGameInfoWidget::HandleLeftButtonClicked()
{
	if (!DataSubsystem || !DataSubsystem->GameFlowInfo)
	{
		return;
	}

	if (PageRowNames.Num() == 0)
	{
		return;
	}

	CurrentPageIndex = (CurrentPageIndex - 1 + PageRowNames.Num()) % PageRowNames.Num();
	UpdatePage(CurrentPageIndex);
}

void URSGameInfoWidget::HandleRightButtonClicked()
{
	if (!DataSubsystem || !DataSubsystem->GameFlowInfo)
	{
		return;
	}

	if (PageRowNames.Num() == 0)
	{
		return;
	}

	CurrentPageIndex = (CurrentPageIndex + 1) % PageRowNames.Num();
	UpdatePage(CurrentPageIndex);
}

void URSGameInfoWidget::UpdatePage(int32 PageIndex)
{
	if (!DataSubsystem || !DataSubsystem->GameFlowInfo)
	{
		return;
	}

	if (!PageRowNames.IsValidIndex(PageIndex))
	{
		return;
	}

	const FName& RowName = PageRowNames[PageIndex];
	const FGameFlowInfoData* GameFlowInfoData = DataSubsystem->GameFlowInfo->FindRow<FGameFlowInfoData>(RowName, TEXT("UpdatePage"));

	if (!GameFlowInfoData) return;

	CategoryTextBlock->SetText(GameFlowInfoData->CategoryText);
	DescriptionTextBlock->SetText(GameFlowInfoData->DescriptionText);
	PageTextBlock->SetText(GameFlowInfoData->PageText);
	GameInfoImage->SetBrushFromTexture(GameFlowInfoData->GameInfoImage);
}