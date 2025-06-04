// Fill out your copyright notice in the Description page of Project Settings.


#include "RSSendIngredientWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "RSGameInstance.h"

void URSSendIngredientWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &URSSendIngredientWidget::ExitWidget);
	}

	if (OKButton)
	{
		OKButton->OnClicked.AddDynamic(this, &URSSendIngredientWidget::NextStageTravel);
	}

	// TODO : 플레이어의 재료 인벤토리를 띄우고 재료 아이템 일부를 타이쿤으로 보낼 수 있도록 구현한다.
}

void URSSendIngredientWidget::NextStageTravel()
{
	// TODO : 재료를 보낼 수 있는 만큼 골랐는지 확인 후 적게 골랐다면 경고창을 띄운다.

	URSGameInstance* GameInstance = GetGameInstance<URSGameInstance>();
	if (GameInstance)
	{
		GameInstance->TravelToLevel(TargetLevelAsset);
	}
}

void URSSendIngredientWidget::ExitWidget()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);
		PC->FlushPressedKeys();
	}

	RemoveFromParent();
}
