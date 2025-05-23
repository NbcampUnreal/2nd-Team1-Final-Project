// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionMenuWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "OptionMenuWidget"

void UOptionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnBackButtonClicked);
	}

	if (ResolutionComboBox)
	{
		ResolutionComboBox->ClearOptions();
		ResolutionComboBox->AddOption(TEXT("1920x1080"));
		ResolutionComboBox->AddOption(TEXT("1280x720"));
		ResolutionComboBox->AddOption(TEXT("800x600"));
		ResolutionComboBox->SetSelectedIndex(0);
	}

	if (WindowModeComboBox)
	{
		WindowModeComboBox->ClearOptions();
		WindowModeComboBox->AddOption(TEXT("Fullscreen"));
		WindowModeComboBox->AddOption(TEXT("WindowedFullscreen"));
		WindowModeComboBox->AddOption(TEXT("Windowed"));
		WindowModeComboBox->SetSelectedIndex(0);
	}

	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnApplyButtonClicked);
	}
}

void UOptionMenuWidget::OnBackButtonClicked()
{
	RemoveFromParent();
}

void UOptionMenuWidget::OnApplyButtonClicked()
{
	if (!ResolutionComboBox || !WindowModeComboBox) return;

	FString SelectedResolution = ResolutionComboBox->GetSelectedOption();
	FString SelectedWindowMode = WindowModeComboBox->GetSelectedOption();

	//Resolution ComboBox ����
	int32 Width;
	int32 Height;

	FString Left, Right;
	if (SelectedResolution.Split(TEXT("x"), &Left, &Right))
	{
		Width = FCString::Atoi(*Left);
		Height = FCString::Atoi(*Right);
	}

	//WindowMode ComboBox ����
	EWindowMode::Type WindowMode;
	if (SelectedWindowMode == TEXT("Fullscreen"))
		WindowMode = EWindowMode::Fullscreen;
	else if (SelectedWindowMode == TEXT("WindowedFullscreen"))
		WindowMode = EWindowMode::WindowedFullscreen;
	else if (SelectedWindowMode == TEXT("Windowed"))
		WindowMode = EWindowMode::Windowed;

	//���� ����
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (Settings)
	{
		Settings->SetScreenResolution(FIntPoint(Width, Height));
		Settings->SetFullscreenMode(WindowMode);
		Settings->ApplySettings(false);
		Settings->SaveSettings();
	}

}