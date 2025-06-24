// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "RSGameInstance.h"
#include "RSSaveGameSubsystem.h"
#include "RSLevelSubsystem.h"
#include "RSGuideWidget.h"


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartButtonClicked);
	}

	// 불러올 세이브 파일이 하나도 없는 경우 불러오기 버튼을 숨기고 해당 버튼의 위치에 StartButton을 위치하게 한다.
	URSGameInstance* RSGameInstance = Cast<URSGameInstance>(GetWorld()->GetGameInstance());
	if (LoadButton && StartButton && RSGameInstance)
	{
		// 던전에 대한 모든 세이브 파일이 있는지 확인한다.
		URSSaveGameSubsystem* SaveGameSubsystem = RSGameInstance->GetSubsystem<URSSaveGameSubsystem>();
		if (SaveGameSubsystem)
		{
			bool bDungeonSaveFileExists = SaveGameSubsystem->DoesDungeonSaveFileExist();
			bool bTycoonSaveFileExists = SaveGameSubsystem->DoesTycoonSaveFileExist();

			// 던전이나 타이쿤에 대한 세이브 파일이 존재 하는 경우
			if (bDungeonSaveFileExists || bTycoonSaveFileExists)
			{
				LoadButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadButtonClicked);
			}
			// 존재하지 않는 경우
			else
			{
				LoadButton->SetVisibility(ESlateVisibility::Hidden);

				UCanvasPanelSlot* LoadButtonSlot = Cast<UCanvasPanelSlot>(LoadButton->Slot);
				if (LoadButtonSlot)
				{
					FVector2D LoadButtonPosition = LoadButtonSlot->GetPosition();

					UCanvasPanelSlot* StartButtonSlot = Cast<UCanvasPanelSlot>(StartButton->Slot);
					if (StartButtonSlot)
					{
						StartButtonSlot->SetPosition(LoadButtonPosition);
					}
				}
			}
		}
	}

	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionButtonClicked);
	}

	if (GuideButton)
	{
		GuideButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnGuideButtonClicked);
		GuideButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HideMainMenu);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
	}
}

void UMainMenuWidget::VsibleMainMenu()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UMainMenuWidget::HideMainMenu()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuWidget::OnGuideButtonClicked()
{
	APlayerController* PC = GetOwningPlayer();

	// 위젯이 생성된 상태인 경우
	if (PC && GuideWidgetInstance)
	{
		GuideWidgetInstance->AddToViewport();

		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		PC->FlushPressedKeys();

		return;
	}

	// 생성된 위젯이 없는 경우
	if (PC && GuideWidgetClass)
	{
		GuideWidgetInstance = CreateWidget<URSGuideWidget>(PC, GuideWidgetClass);

		GuideWidgetInstance->AddToViewport();

		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		PC->FlushPressedKeys();

		UButton* GuideWidgetCloswButton = GuideWidgetInstance->GetCloseButton();
		if (GuideWidgetCloswButton)
		{
			GuideWidgetCloswButton->OnClicked.AddDynamic(this, &UMainMenuWidget::VsibleMainMenu);
		}
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
		URSLevelSubsystem* LevelSubsystem = RSGameInstance->GetSubsystem<URSLevelSubsystem>();
		if (LevelSubsystem)
		{
			LevelSubsystem->TravelToLevel(ERSLevelCategory::BaseArea);
		}
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
				URSLevelSubsystem* LevelSubsystem = RSGameInstance->GetSubsystem<URSLevelSubsystem>();
				if (LevelSubsystem)
				{
					LevelSubsystem->TravelToLevel(ERSLevelCategory::Dungeon);
				}
			}
			// 존재하지 않는 경우
			else
			{
				// 거점으로 레벨 이동
				URSLevelSubsystem* LevelSubsystem = RSGameInstance->GetSubsystem<URSLevelSubsystem>();
				if (LevelSubsystem)
				{
					LevelSubsystem->TravelToLevel(ERSLevelCategory::BaseArea);
				}
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

