// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "RSGameInstance.h"
#include "RSSaveGameSubsystem.h"


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartButtonClicked);
	}

	// TODO : 만약 불러올 세이브 파일이 하나도 없는 경우 해당 버튼이 보이거나 위치하면 안된다.
	// 해당 버튼을 숨기고 로드 버튼의 위치에 StartButton을 위치하게 한다.
	if (LoadButton)
	{
		LoadButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadButtonClicked);
	}

	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionButtonClicked);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
	}
}

void UMainMenuWidget::OnStartButtonClicked()
{
	URSGameInstance* RSGameInstance = Cast<URSGameInstance>(GetWorld()->GetGameInstance());
	
	if (RSGameInstance)
	{
		// 기존 모든 세이브 제거
		URSSaveGameSubsystem* SaveGameSubsystem = RSGameInstance->GetSubsystem<URSSaveGameSubsystem>();
		if (SaveGameSubsystem)
		{
			SaveGameSubsystem->DeleteAllSaveFile();
		}

		// 레벨 이동
		RSGameInstance->TravelToLevel(NewGameTargetLevelAsset);
	}
}

void UMainMenuWidget::OnLoadButtonClicked()
{
	URSGameInstance* RSGameInstance = Cast<URSGameInstance>(GetWorld()->GetGameInstance());
	if (RSGameInstance)
	{
		// 던전에 대한 모든 세이브 파일이 있는지 확인한다.
		URSSaveGameSubsystem* SaveGameSubsystem = RSGameInstance->GetSubsystem<URSSaveGameSubsystem>();
		if (SaveGameSubsystem)
		{
			bool bDungeonSaveFileExists = SaveGameSubsystem->DoesDungeonSaveFileExist();

			// 던전에 대한 세이브 파일이 모두 존재 하는 경우
			if (bDungeonSaveFileExists)
			{
				// 던전으로 레벨 이동
				//RSGameInstance->TravelToLevel(NewGameTargetLevelAsset);
			}
			// 존재하지 않는 경우
			else
			{
				// 거점으로 레벨 이동
				//RSGameInstance->TravelToLevel(NewGameTargetLevelAsset);
			}
		}
	}
}

void UMainMenuWidget::OnOptionButtonClicked()
{
	if (OptionMenuWidgetClass)
	{
		UUserWidget* OptionMenu = CreateWidget<UUserWidget>(GetWorld(), OptionMenuWidgetClass);
		if (OptionMenu)
		{
			OptionMenu->AddToViewport();
		}
	}
}

void UMainMenuWidget::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

