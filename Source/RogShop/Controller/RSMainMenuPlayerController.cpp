// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Option/OptionSaveGame.h"
#include "RSSaveGameSubsystem.h"
#include "RSGameInstance.h"

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

    LoadSetting();
}

void ARSMainMenuPlayerController::LoadSetting()
{
    UOptionSaveGame* LoadedData = nullptr;
    URSSaveGameSubsystem* SaveGameSubsystem = GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>();
    if (SaveGameSubsystem)
    {
        if (UGameplayStatics::DoesSaveGameExist(SaveGameSubsystem->OptionSaveSlotName, 0))
        {
            LoadedData = Cast<UOptionSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSubsystem->OptionSaveSlotName, 0));
        }
    }

    if (LoadedData)
    {
        URSGameInstance* GameInstance = GetGameInstance<URSGameInstance>();
        if (GameInstance)
        {
            GameInstance->OnMasterVolumeChanged(LoadedData->MasterVolume);

            GameInstance->OnBGMVolumeChanged(LoadedData->BGMVolume);

            GameInstance->OnSFXVolumeChanged(LoadedData->SFXVolume);
        }
    }
}
