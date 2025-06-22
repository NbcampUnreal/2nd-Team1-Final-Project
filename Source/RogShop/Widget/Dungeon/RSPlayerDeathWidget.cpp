// Fill out your copyright notice in the Description page of Project Settings.


#include "RSPlayerDeathWidget.h"
#include "Components/Button.h"
#include "RSGameInstance.h"
#include "RSLevelSubsystem.h"

void URSPlayerDeathWidget::NativeConstruct()
{
	if (BaseAreaButton)
	{
		BaseAreaButton->OnClicked.AddDynamic(this, &URSPlayerDeathWidget::OnBaseAreaButtonClicked);
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->SetShowMouseCursor(true);

		// InputMode 변경 및 현재 위젯에 포커스
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(this->TakeWidget());
		PC->SetInputMode(InputMode);

		PC->FlushPressedKeys();
	}
}

void URSPlayerDeathWidget::OnBaseAreaButtonClicked()
{
	// TODO : 기존 던전 세이브 제거

	// 레벨 이동
	URSGameInstance* RSGameInstance = Cast<URSGameInstance>(GetWorld()->GetGameInstance());
	if (RSGameInstance)
	{
		URSLevelSubsystem* LevelSubsystem = RSGameInstance->GetSubsystem<URSLevelSubsystem>();

		if (LevelSubsystem)
		{
			LevelSubsystem->TravelToLevel(ERSLevelCategory::BaseArea);
		}
	}
}
