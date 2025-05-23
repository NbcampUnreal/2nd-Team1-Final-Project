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
    // ���� �߰�
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Subsystem�� ���� �츮�� �Ҵ��� IMC�� Ȱ��ȭ
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
    // ��� ���� ����
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
