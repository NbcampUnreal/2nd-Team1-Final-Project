// Fill out your copyright notice in the Description page of Project Settings.


#include "DunShopWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void UDunShopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UE_LOG(LogTemp, Warning, TEXT("DunShopWidget Open !"));

    SetMouseMode(true);

    if (ExitBtn)
    {
        ExitBtn->OnClicked.AddDynamic(this, &UDunShopWidget::OnExitClicked);
    }

    const int32 ItemCount = 5;

    for (int32 i = 0; i < ItemCount; ++i)
    {
        if (ItemWidgetClass && ItemHorizontalBox)
        {
            UUserWidget* NewItemWidget = CreateWidget<UUserWidget>(this, ItemWidgetClass);

            if (NewItemWidget)
            {
                // AddChild 후 슬롯을 받아서 Fill 설정
                UHorizontalBoxSlot* BoxSlot = Cast<UHorizontalBoxSlot>(ItemHorizontalBox->AddChild(NewItemWidget));

                if (BoxSlot)
                {
                    BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
                }
            }
        }
    }

}

void UDunShopWidget::OnExitClicked()
{
    SetMouseMode(false);

    RemoveFromParent();
}

void UDunShopWidget::SetMouseMode(bool bEnable)
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