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

    LoadSetting();

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

    float Master = 1.f;
    float BGM = 1.f;
    float SFX = 1.f;

    if (LoadedData)
    {
        Master = LoadedData->MasterVolume;

        BGM = LoadedData->BGMVolume;

        SFX = LoadedData->SFXVolume;
    }

    URSGameInstance* GameInstance = GetGameInstance<URSGameInstance>();
    if (GameInstance)
    {
        GameInstance->OnMasterVolumeChanged(Master);

        GameInstance->OnBGMVolumeChanged(BGM);

        GameInstance->OnSFXVolumeChanged(SFX);
    }
}
