// Fill out your copyright notice in the Description page of Project Settings.

#include "RSDunPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "RSCheatManager.h"
#include "RSDunMainHUDWidget.h"
#include "RSPlayerInventoryWidget.h"
#include "TimerManager.h"
#include "RSDunPlayerCharacter.h"
#include "RSDungeonGameModeBase.h"

ARSDunPlayerController::ARSDunPlayerController()
{
    CheatClass = URSCheatManager::StaticClass();

}

void ARSDunPlayerController::BeginPlay()
{
    Super::BeginPlay();
    ShowLoadingUI();

    // 마우스 커서를 감추고, Input모드를 게임온리로 변경
    bShowMouseCursor = false;

    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);

    // 기본적인 입력 매핑 추가
    AddMapping();

    InitializeRSDunMainWidget();

    BindCharacterDelegates();
}

void ARSDunPlayerController::AddMapping()
{
    // 매핑 추가
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Subsystem을 통해 우리가 할당한 IMC를 활성화
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (IMC)
            {
                Subsystem->AddMappingContext(IMC, 0);
            }
        }
    }
}

void ARSDunPlayerController::RemoveAllMapping()
{
    // 모든 매핑 제거
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            Subsystem->ClearAllMappings();
        }
    }
}

void ARSDunPlayerController::InitializeRSDunMainWidget()
{
    if (RSDunMainHUDWidgetClass)
    {
        RSDunMainHUDWidget = CreateWidget<URSDunMainHUDWidget>(this, RSDunMainHUDWidgetClass);

        if (RSDunMainHUDWidget)
        {
            RSDunMainHUDWidget->AddToViewport();
        }
    }
}

void ARSDunPlayerController::TogglePlayerInventoryWidget()
{
    if (RSDunMainHUDWidget)
    {
        RSDunMainHUDWidget->HandlePlayerInventoryWidget();
    }
}

void ARSDunPlayerController::ToggleInGameMenuWidget()
{
    if (RSDunMainHUDWidget)
    {
        RSDunMainHUDWidget->HandleInGameMenuWidget();
    }
}

void ARSDunPlayerController::ShowLoadingUI()
{
    ARSDungeonGameModeBase* DungeonGameModeBase = GetWorld()->GetAuthGameMode<ARSDungeonGameModeBase>();

    // 던전 게임모드일 경우에만 로딩창이 그려지도록 구현
    if (DungeonGameModeBase)
    {
        if (LoadingUIWidgetClass)
        {
            LoadingUIWidget = CreateWidget<UUserWidget>(this, LoadingUIWidgetClass);

            if (LoadingUIWidget)
            {
                LoadingUIWidget->AddToViewport(999);
            }
        }
    }
}

void ARSDunPlayerController::ShowPlayerDeathWidget()
{
    if (RSDeathWidgetClass)
    {
        RSDeathWidget = CreateWidget<UUserWidget>(this, RSDeathWidgetClass);

        if (RSDeathWidget)
        {
            RSDeathWidget->AddToViewport();
        }

        if (RSDunMainHUDWidget)
        {
            RSDunMainHUDWidget->HideAllWidgets();
        }
    }
}

void ARSDunPlayerController::ShowInteractWidget()
{
    if (RSDunMainHUDWidget)
    {
        RSDunMainHUDWidget->ShowInteractWidget();
    }
}

void ARSDunPlayerController::HideInteractWidget()
{
    if (RSDunMainHUDWidget)
    {
        RSDunMainHUDWidget->HideInteractWidget();
    }
}

void ARSDunPlayerController::BindCharacterDelegates()
{
    ARSDunPlayerCharacter* CurPawn = GetPawn<ARSDunPlayerCharacter>();
    if (CurPawn)
    {
        CurPawn->OnCharacterDied.AddDynamic(this, &ARSDunPlayerController::ShowPlayerDeathWidget);
    }

}
