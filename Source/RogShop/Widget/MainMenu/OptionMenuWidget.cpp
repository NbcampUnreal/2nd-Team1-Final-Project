#include "OptionMenuWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Sound/SoundClass.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "Option/OptionSaveGame.h"
#include "RSSaveGameSubsystem.h"

void UOptionMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BackButton)
        BackButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnBackButtonClicked);

    if (ApplyButton)
        ApplyButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnApplyButtonClicked);

    if (MasterVolumeSlider)
        MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionMenuWidget::OnMasterVolumeChanged);

    if (BGMVolumeSlider)
        BGMVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionMenuWidget::OnBGMVolumeChanged);

    if (SFXVolumeSlider)
        SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionMenuWidget::OnSFXVolumeChanged);

    if (ResolutionComboBox)
    {
        ResolutionComboBox->ClearOptions();
        ResolutionComboBox->AddOption(TEXT("1920x1080"));
        ResolutionComboBox->AddOption(TEXT("1280x720"));
        ResolutionComboBox->AddOption(TEXT("800x600"));
    }

    if (WindowModeComboBox)
    {
        WindowModeComboBox->ClearOptions();
        WindowModeComboBox->AddOption(TEXT("Fullscreen"));
        WindowModeComboBox->AddOption(TEXT("WindowedFullscreen"));
        WindowModeComboBox->AddOption(TEXT("Windowed"));
    }

    LoadSettings();
}

void UOptionMenuWidget::LoadSettings()
{
    UOptionSaveGame* LoadedData = nullptr;
    URSSaveGameSubsystem* SaveGameSubsystem = GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>();
    if (UGameplayStatics::DoesSaveGameExist(SaveGameSubsystem->OptionSaveSlotName, 0))
    {
        LoadedData = Cast<UOptionSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSubsystem->OptionSaveSlotName, 0));
    }

    if (LoadedData)
    {
        if (MasterVolumeSlider)
            MasterVolumeSlider->SetValue(LoadedData->MasterVolume);
        if (BGMVolumeSlider)
            BGMVolumeSlider->SetValue(LoadedData->BGMVolume);
        if (SFXVolumeSlider)
            SFXVolumeSlider->SetValue(LoadedData->SFXVolume);
        if (ResolutionComboBox)
            ResolutionComboBox->SetSelectedIndex(LoadedData->ResolutionIndex);
        if (WindowModeComboBox)
            WindowModeComboBox->SetSelectedIndex(LoadedData->WindowModeIndex);
    }
    else
    {
        if (ResolutionComboBox)
            ResolutionComboBox->SetSelectedIndex(0);
        if (WindowModeComboBox)
            WindowModeComboBox->SetSelectedIndex(0);
    }
}


void UOptionMenuWidget::SaveSettings()
{
    UOptionSaveGame* SaveGameInstance = Cast<UOptionSaveGame>(
        UGameplayStatics::CreateSaveGameObject(UOptionSaveGame::StaticClass()));

    if (MasterVolumeSlider)
        SaveGameInstance->MasterVolume = MasterVolumeSlider->GetValue();
    if (BGMVolumeSlider)
        SaveGameInstance->BGMVolume = BGMVolumeSlider->GetValue();
    if (SFXVolumeSlider)
        SaveGameInstance->SFXVolume = SFXVolumeSlider->GetValue();
    if (ResolutionComboBox)
        SaveGameInstance->ResolutionIndex = ResolutionComboBox->GetSelectedIndex();
    if (WindowModeComboBox)
        SaveGameInstance->WindowModeIndex = WindowModeComboBox->GetSelectedIndex();

    URSSaveGameSubsystem* SaveGameSubsystem = GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>();
    if (SaveGameSubsystem)
    {
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameSubsystem->OptionSaveSlotName, 0);
    }
}

void UOptionMenuWidget::OnApplyButtonClicked()
{
    if (!ResolutionComboBox || !WindowModeComboBox) return;

    FString SelectedResolution = ResolutionComboBox->GetSelectedOption();
    FString SelectedWindowMode = WindowModeComboBox->GetSelectedOption();

    int32 Width = 1280;
    int32 Height = 720;

    FString Left, Right;
    if (SelectedResolution.Split(TEXT("x"), &Left, &Right))
    {
        Width = FCString::Atoi(*Left);
        Height = FCString::Atoi(*Right);
    }

    EWindowMode::Type WindowMode = EWindowMode::Windowed;
    if (SelectedWindowMode == TEXT("Fullscreen"))
        WindowMode = EWindowMode::Fullscreen;
    else if (SelectedWindowMode == TEXT("WindowedFullscreen"))
        WindowMode = EWindowMode::WindowedFullscreen;

    UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    if (Settings)
    {
        Settings->SetScreenResolution(FIntPoint(Width, Height));
        Settings->SetFullscreenMode(WindowMode);
        Settings->ApplySettings(false);
        Settings->SaveSettings();
    }

    SaveSettings();
}

void UOptionMenuWidget::OnBackButtonClicked()
{
    RemoveFromParent();
}

void UOptionMenuWidget::OnMasterVolumeChanged(float Value)
{
    if (SC_Master)
    {
        SC_Master->Properties.Volume = Value;
    }
}

void UOptionMenuWidget::OnBGMVolumeChanged(float Value)
{
    if (SC_BGM)
    {
        SC_BGM->Properties.Volume = Value;
    }
}

void UOptionMenuWidget::OnSFXVolumeChanged(float Value)
{
    if (SC_SFX)
    {
        SC_SFX->Properties.Volume = Value;
    }
}
