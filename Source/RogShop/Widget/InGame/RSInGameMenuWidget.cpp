// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInGameMenuWidget.h"
#include "Components/Button.h"
#include "RSGameInstance.h"
#include "RSLevelSubsystem.h"
#include "RSSaveGameSubsystem.h"
#include "RSDunPlayerController.h"
#include "RSGuideWidget.h"

void URSInGameMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (GuideButton)
	{
		GuideButton->OnClicked.AddDynamic(this, &URSInGameMenuWidget::OnGuideButtonClicked);
		GuideButton->OnClicked.AddDynamic(this, &URSInGameMenuWidget::HideInGameMenu);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &URSInGameMenuWidget::OnMainMenuButtonClicked);
	}

	if (BaseAreaButton)
	{
		BaseAreaButton->OnClicked.AddDynamic(this, &URSInGameMenuWidget::OnBaseAreaButtonClicked);
	}
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &URSInGameMenuWidget::OnCloseButtonClicked);
	}
}

void URSInGameMenuWidget::VsibleInGameMenu()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void URSInGameMenuWidget::HideInGameMenu()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void URSInGameMenuWidget::OnGuideButtonClicked()
{
	APlayerController* PC = GetOwningPlayer();

	// 위젯이 생성된 상태인 경우
	if (PC && GuideWidgetInstance)
	{
		GuideWidgetInstance->AddToViewport();

		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		PC->FlushPressedKeys();

		return;
	}

	// 생성된 위젯이 없는 경우
	if (PC && GuideWidgetClass)
	{
		GuideWidgetInstance = CreateWidget<URSGuideWidget>(PC, GuideWidgetClass);
		
		GuideWidgetInstance->AddToViewport();

		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		PC->FlushPressedKeys();

		UButton* GuideWidgetCloswButton = GuideWidgetInstance->GetCloseButton();
		if (GuideWidgetCloswButton)
		{
			GuideWidgetCloswButton->OnClicked.AddDynamic(this, &URSInGameMenuWidget::VsibleInGameMenu);
		}
	}
}

void URSInGameMenuWidget::OnMainMenuButtonClicked()
{
	// 레벨 이동
	URSGameInstance* RSGameInstance = Cast<URSGameInstance>(GetWorld()->GetGameInstance());
	if (RSGameInstance)
	{
		URSLevelSubsystem* LevelSubsystem = RSGameInstance->GetSubsystem<URSLevelSubsystem>();

		if (LevelSubsystem)
		{
			LevelSubsystem->TravelToLevel(ERSLevelCategory::MainMenu);
		}
	}
}

void URSInGameMenuWidget::OnBaseAreaButtonClicked()
{
	// 던전 세이브 파일 제거
	UGameInstance* CurGameInstance = GetGameInstance();
	if (CurGameInstance)
	{
		URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
		if (SaveGameSubsystem)
		{
			SaveGameSubsystem->DeleteDungeonSaveFile();
		}
	}

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

void URSInGameMenuWidget::OnCloseButtonClicked()
{
	// 메뉴 닫기
	if (GetWorld())
	{
		ARSDunPlayerController* PC = GetOwningPlayer<ARSDunPlayerController>();
		if (PC)
		{
			PC->ToggleInGameMenuWidget();
		}
	}
}
