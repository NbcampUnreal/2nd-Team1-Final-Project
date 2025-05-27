// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "RSGameInstance.h"


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartButtonClicked);
	}

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
	// TODO : 기존 세이브 제거

	// 레벨 이동
	URSGameInstance* RSGameInstance = Cast<URSGameInstance>(GetWorld()->GetGameInstance());
	if (RSGameInstance)
	{
		RSGameInstance->TravelToLevel(NewGameTargetLevelAsset);
	}
}

void UMainMenuWidget::OnLoadButtonClicked()
{
	//UGameplayStatics::OpenLevel(this, FName("다음레벨 이름"));
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

