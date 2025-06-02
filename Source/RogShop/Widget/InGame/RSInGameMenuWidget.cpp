// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInGameMenuWidget.h"
#include "Components/Button.h"
#include "RSGameInstance.h"

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
}

void URSInGameMenuWidget::OnMainMenuButtonClicked()
{
	// 레벨 이동
	URSGameInstance* RSGameInstance = Cast<URSGameInstance>(GetWorld()->GetGameInstance());
	if (RSGameInstance)
	{
		RSGameInstance->TravelToLevel(MainMenuLevelAsset);
	}
}

void URSInGameMenuWidget::OnBaseAreaButtonClicked()
{
	// TODO : 던전에서는 기존 던전 세이브 제거

	// 레벨 이동
	URSGameInstance* RSGameInstance = Cast<URSGameInstance>(GetWorld()->GetGameInstance());
	if (RSGameInstance)
	{
		RSGameInstance->TravelToLevel(BaseAreaLevelAsset);
	}
}
