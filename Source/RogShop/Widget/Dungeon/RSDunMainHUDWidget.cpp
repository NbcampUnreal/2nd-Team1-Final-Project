// Fill out your copyright notice in the Description page of Project Settings.

#include "RSDunMainHUDWidget.h"
#include "RSInGameMenuWidget.h"
#include "RSPlayerStatusWidget.h"
#include "RSBossHPBarWidget.h"
#include "RSInteractWidget.h"
#include "RSPlayerInventoryWidget.h"
#include "RSDungeonGameModeBase.h"
#include "EngineUtils.h"
#include "RSMapGenerator.h"
#include "RSMiniMap.h"
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

    if (MiniMapWidget)
    {

        if (UWorld* World = GetWorld())
        {
            if (ARSDungeonGameModeBase* DungeonGM = Cast<ARSDungeonGameModeBase>(UGameplayStatics::GetGameMode(World)))
            {
                if (ARSMapGenerator* MapGen = DungeonGM->GetMapGenerator())
                {
                    MiniMapWidget->InitializeMap(
                        MapGen->GetAllTileCoords(),
                        MapGen->GetBossTileCoord()
                    );
                    UE_LOG(LogTemp, Warning, TEXT("[HUD] 미니맵 초기화 완료"));
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("[HUD] 미니맵 초기화 실패"));
                }
            }
        }
    }
}

void URSDunMainHUDWidget::HideAllWidgets()
{
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
        PlayerStatusWidget->SetVisibility(ESlateVisibility::Hidden);
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
        if (PlayerInventoryWidget->IsVisible())
        {
            PlayerInventoryWidget->SetVisibility(ESlateVisibility::Hidden);

            FInputModeGameOnly InputMode;
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = false;
            PC->FlushPressedKeys();

            // 상호작용 UI가 나타나도록 합니다.
            ShowInteractWidget();
        }
        else
        {
            PlayerInventoryWidget->SetVisibility(ESlateVisibility::Visible);

            FInputModeUIOnly InputMode;
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
            PC->FlushPressedKeys();

            // 상호작용 UI를 숨깁니다.
            HideInteractWidget();
        }
    }
}

void URSDunMainHUDWidget::ShowInteractWidget()
{
    if (InteractWidget)
    {
        InteractWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
    }
}

void URSDunMainHUDWidget::HideInteractWidget()
{
    if (InteractWidget)
    {
        InteractWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void URSDunMainHUDWidget::UpdateMiniMapPlayerPosition(const FIntPoint& TileCoord) //플레이어가 다른 타일에 들어갔음을때 미니맵을 업데이트 시킬 함수
{
    if (MiniMapWidget)
    {
        MiniMapWidget->UpdatePlayerPosition(TileCoord);
    }
}



