// Fill out your copyright notice in the Description page of Project Settings.

#include "RSDunPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "RSCheatManager.h"
#include "RSDunMainHUDWidget.h"
#include "RSPlayerInventoryWidget.h"

ARSDunPlayerController::ARSDunPlayerController()
{
    CheatClass = URSCheatManager::StaticClass();

}

void ARSDunPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 마우스 커서를 감추고, Input모드를 게임온리로 변경
    bShowMouseCursor = false;

    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);

    // 기본적인 입력 매핑 추가
    AddMapping();

    InitializeRSDunMainWidget();
    InitializeRSPlayerInventoryWidget();
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
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("RSDunMainHUDWidgetClass Null !"));
    }
}

void ARSDunPlayerController::InitializeRSPlayerInventoryWidget()
{
    if (RSPlayerInventoryWidgetClass)
    {
        RSPlayerInventoryWidgetInstance = CreateWidget<URSPlayerInventoryWidget>(this, RSPlayerInventoryWidgetClass);

        if (RSPlayerInventoryWidgetInstance)
        {
            RSPlayerInventoryWidgetInstance->AddToViewport();
            RSPlayerInventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden); // 시작은 숨김
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("RSPlayerInventoryWidget Null !"));
    }
}

void ARSDunPlayerController::AddRSPlayerInventoryWidget()
{
    if (RSPlayerInventoryWidgetInstance->IsVisible())
    {
        RSPlayerInventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden);

        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);
        bShowMouseCursor = false;
    }
    else
    {
        RSPlayerInventoryWidgetInstance->SetVisibility(ESlateVisibility::Visible);

        FInputModeUIOnly InputMode;
        SetInputMode(InputMode);
        bShowMouseCursor = true;
    }
}

void ARSDunPlayerController::ToggleInGameMenuWidget()
{
    if (RSDunMainHUDWidget)
    {
        RSDunMainHUDWidget->HandleInGameMenuWidget();
    }
}
