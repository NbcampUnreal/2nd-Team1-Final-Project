// Fill out your copyright notice in the Description page of Project Settings.

#include "RSDunMainHUDWidget.h"
#include "RSInGameMenuWidget.h"
#include "RSPlayerStatusWidget.h"
#include "GameFramework/PlayerController.h"

void URSDunMainHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (InGameMenuWidget)
    {
        InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    
    if (PlayerStatusWidget)
    {
        PlayerStatusWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    
}

void URSDunMainHUDWidget::HandleInGameMenuWidget()
{
    // 다른 UI를 보이지 않도록 설정하고, 인게임 메뉴 UI를 보이도록 Visibility 설정 변경
    // 현재 다른 UI는 DunMainWidget
    if (InGameMenuWidget && PlayerStatusWidget)
    {
        ESlateVisibility InGameMenuVisibility = InGameMenuWidget->GetVisibility();

        if (InGameMenuVisibility == ESlateVisibility::SelfHitTestInvisible)
        {
            InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
            PlayerStatusWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

            APlayerController* PC = GetOwningPlayer();
            if (PC)
            {
                PC->SetShowMouseCursor(false);

                // InputMode 변경
                FInputModeGameOnly InputMode;
                PC->SetInputMode(InputMode);

                PC->FlushPressedKeys();
            }
        }
        else if (InGameMenuVisibility == ESlateVisibility::Hidden)
        {
            InGameMenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
            PlayerStatusWidget->SetVisibility(ESlateVisibility::Hidden);

            APlayerController* PC = GetOwningPlayer();
            if (PC)
            {
                PC->SetShowMouseCursor(true);

                // InputMode 변경 및 현재 위젯에 포커스
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(this->TakeWidget());
                PC->SetInputMode(InputMode);

                PC->FlushPressedKeys();
            }
        }
    }
}
