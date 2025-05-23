// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartButtonClicked);
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
	FGenericPlatformMisc::RequestExit(false);
}

