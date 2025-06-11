// Fill out your copyright notice in the Description page of Project Settings.

#include "RSDunMainHUDWidget.h"
#include "RSInGameMenuWidget.h"
#include "RSPlayerStatusWidget.h"
#include "RSPlayerInventoryWidget.h"
#include "GameFramework/PlayerController.h"

void URSDunMainHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (InGameMenuWidget)
    {
        InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    
    if (PlayerInventoryWidget)
    {
        PlayerInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (PlayerStatusWidget)
    {
        PlayerStatusWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    if (InteractWidget)
    {
		InteractWidget->SetVisibility(ESlateVisibility::Hidden);
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

void URSDunMainHUDWidget::HandlePlayerInventoryWidget()
{
    APlayerController* PC = GetOwningPlayer();

    if (PC)
    {
        // 이 창을 끄는건 Exit 버튼으로 해서 이 부분은 현재 실행 안 됨, 방어 코드
        if (PlayerInventoryWidget->IsVisible())
        {
            PlayerInventoryWidget->SetVisibility(ESlateVisibility::Hidden);

            FInputModeGameOnly InputMode;
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = false;
            PC->FlushPressedKeys();
        }
        // 현재는 위젯을 켜기만 해서 이 부분만 계속 실행
        else
        {
            PlayerInventoryWidget->SetVisibility(ESlateVisibility::Visible);

            FInputModeUIOnly InputMode;
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
            PC->FlushPressedKeys();
        }
    }
}
