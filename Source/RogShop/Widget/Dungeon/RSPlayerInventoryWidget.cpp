// Fill out your copyright notice in the Description page of Project Settings.

#include "RSPlayerInventoryWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "RSDunPlayerController.h"

void URSPlayerInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ExitBtn)
    {
        ExitBtn->OnClicked.AddDynamic(this, &URSPlayerInventoryWidget::OnExitBtnClicked);
    }

    ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();

    if (RSDunPlayerController)
    {
        RSDunPlayerController->OnLifeEssenceChange.AddDynamic(this, &URSPlayerInventoryWidget::UpdateLifeEssence);
    }
}

void URSPlayerInventoryWidget::UpdateLifeEssence(int32 NewLifeEssence)
{
    if (LifeEssenceQuantityText)
    {
        FText LifeEssenceText = FText::AsNumber(NewLifeEssence); // 정수 형태로 변환
        LifeEssenceQuantityText->SetText(LifeEssenceText);
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
