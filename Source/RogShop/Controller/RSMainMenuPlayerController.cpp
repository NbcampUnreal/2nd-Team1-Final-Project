// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"

void ARSMainMenuPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();

			bShowMouseCursor = true;

			FInputModeUIOnly InputMode;

			//UI 위젯에 포커스
			InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
			
			//마우스 자유롭게
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);

		}
	}
}