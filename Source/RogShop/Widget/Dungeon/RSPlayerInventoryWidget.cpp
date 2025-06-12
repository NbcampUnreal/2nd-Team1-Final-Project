// Fill out your copyright notice in the Description page of Project Settings.

#include "RSPlayerInventoryWidget.h"

#include "Components/Button.h"
#include "RSDunPlayerController.h"

void URSPlayerInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ExitBtn)
    {
        ExitBtn->OnClicked.AddDynamic(this, &URSPlayerInventoryWidget::OnExitBtnClicked);
    }
}

void URSPlayerInventoryWidget::OnExitBtnClicked()
{
    // 인벤토리 닫기
    if (GetWorld())
    {
        ARSDunPlayerController* PC = GetOwningPlayer<ARSDunPlayerController>();
        if (PC)
        {
            PC->TogglePlayerInventoryWidget();
        }
    }
}

void URSPlayerInventoryWidget::SetMouseMode(bool bEnable)
{
    UWorld* World = GetWorld();

    if (World)
    {
        APlayerController* PlayerController = World->GetFirstPlayerController();

        if (PlayerController)
        {
            if (bEnable)
            {
                FInputModeUIOnly InputMode;
                PlayerController->SetInputMode(InputMode);
                PlayerController->bShowMouseCursor = true;
            }
            else
            {
                FInputModeGameOnly InputMode;
                PlayerController->SetInputMode(InputMode);
                PlayerController->bShowMouseCursor = false;
            }
        }
    }
}
