// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInGameMenuWidget.h"
#include "Components/Button.h"
#include "RSGameInstance.h"
#include "RSLevelSubsystem.h"
#include "RSSaveGameSubsystem.h"
#include "RSDunPlayerController.h"

void URSInGameMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

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
