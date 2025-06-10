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

void URSInGameMenuWidget::OnCloseButtonClicked()
{
	// 메뉴 닫기
	if (GetWorld())
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}
