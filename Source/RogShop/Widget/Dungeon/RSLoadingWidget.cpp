// Fill out your copyright notice in the Description page of Project Settings.


#include "RSLoadingWidget.h"
#include "RSDungeonGameModeBase.h"
#include "Components/Image.h"

void URSLoadingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ARSDungeonGameModeBase* DungeonGameModeBase = GetWorld()->GetAuthGameMode<ARSDungeonGameModeBase>();
	if (DungeonGameModeBase)
	{
		DungeonGameModeBase->OnGameReady.AddDynamic(this, &URSLoadingWidget::HideLoading);
	}

	if (LoadingImage)
	{
		int32 ImageIndex = FMath::RandRange(0, BackGroundImages.Num());
		if (BackGroundImages.IsValidIndex(ImageIndex))
		{
			LoadingImage->SetBrushFromTexture(BackGroundImages[ImageIndex]);
		}
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
