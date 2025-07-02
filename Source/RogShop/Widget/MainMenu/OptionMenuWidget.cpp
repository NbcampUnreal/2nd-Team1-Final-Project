#include "OptionMenuWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "Option/OptionSaveGame.h"
#include "RSSaveGameSubsystem.h"
#include "RSGameInstance.h"

void UOptionMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BackButton)
        BackButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnBackButtonClicked);

    if (ApplyButton)
        ApplyButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnApplyButtonClicked);

    URSGameInstance* GameInstance = GetWorld()->GetGameInstance<URSGameInstance>();
    if (GameInstance)
    {
        if (MasterVolumeSlider)
            MasterVolumeSlider->OnValueChanged.AddDynamic(GameInstance, &URSGameInstance::OnMasterVolumeChanged);

        if (BGMVolumeSlider)
            BGMVolumeSlider->OnValueChanged.AddDynamic(GameInstance, &URSGameInstance::OnBGMVolumeChanged);

        if (SFXVolumeSlider)
            SFXVolumeSlider->OnValueChanged.AddDynamic(GameInstance, &URSGameInstance::OnSFXVolumeChanged);
    }


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

    float Master = 1.f;
    float BGM = 1.f;
    float SFX = 1.f;
    int32 Resolution = 0;
    int32 WindowMode = 0;

    if (LoadedData)
    {
        Master = LoadedData->MasterVolume;

        BGM = LoadedData->BGMVolume;

        SFX = LoadedData->SFXVolume;

        Resolution = LoadedData->ResolutionIndex;

        WindowMode = LoadedData->WindowModeIndex;
    }

    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->SetValue(Master);
    }

    if (BGMVolumeSlider)
    {
        BGMVolumeSlider->SetValue(BGM);
    }

    if (SFXVolumeSlider)
    {
        SFXVolumeSlider->SetValue(SFX);
    }

    if (ResolutionComboBox)
    {
        ResolutionComboBox->SetSelectedIndex(Resolution);
    }

    if (WindowModeComboBox)
    {
        WindowModeComboBox->SetSelectedIndex(WindowMode);
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