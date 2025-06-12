// Fill out your copyright notice in the Description page of Project Settings.


#include "RSLoadingWidget.h"
#include "RSDungeonGameModeBase.h"

void URSLoadingWidget::NativeConstruct()
{
	ARSDungeonGameModeBase* DungeonGameModeBase = GetWorld()->GetAuthGameMode<ARSDungeonGameModeBase>();
	if (DungeonGameModeBase)
	{
		DungeonGameModeBase->OnGameReady.AddDynamic(this, &URSLoadingWidget::HideLoading);
	}
}

void URSLoadingWidget::HideLoading()
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
