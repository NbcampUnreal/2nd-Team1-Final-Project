// Fill out your copyright notice in the Description page of Project Settings.

#include "RSDunPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "RSCheatManager.h"
#include "Blueprint/UserWidget.h"

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

    if (RSDunMainWidget)
    {
        OnWeaponSlotChange.AddDynamic(RSDunMainWidget, &URSDunMainWidget::UpdateWeaponSlot);
        OnHPChange.AddDynamic(RSDunMainWidget, &URSDunMainWidget::UpdateHP);
        OnMaxHPChange.AddDynamic(RSDunMainWidget, &URSDunMainWidget::UpdateMaxHP);
    }
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
    if (RSDunMainWidgetClass)
    {
        RSDunMainWidget = CreateWidget<URSDunMainWidget>(this, RSDunMainWidgetClass);

        if (RSDunMainWidget)
        {
            RSDunMainWidget->AddToViewport();
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("RSDunMainWidgetClass Null !"));
    }
}
