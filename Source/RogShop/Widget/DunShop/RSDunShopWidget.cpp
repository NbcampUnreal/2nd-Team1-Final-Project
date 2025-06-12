// Fill out your copyright notice in the Description page of Project Settings.

#include "RSDunShopWidget.h"

#include "RSGameInstance.h"
#include "RSDunItemWidget.h"
#include "RSDataSubsystem.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Kismet/GameplayStatics.h"

#include "RSDunPlayerCharacter.h"
#include "RSDunPlayerController.h"
#include "RogShop/UtilDefine.h"

void URSDunShopWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();

    if (RSDunPlayerController)
    {
        RSDunPlayerController->OnLifeEssenceChange.AddDynamic(this, &URSDunShopWidget::UpdateLifeEssence);
    }
}

void URSDunShopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ExitBtn)
    {
        ExitBtn->OnClicked.AddDynamic(this, &URSDunShopWidget::OnExitClicked);
    }

    SetMouseMode(true);

    PopulateShopItems();
}

void URSDunShopWidget::UpdateLifeEssence(int NewLifeEssence)
{
    if (LifeEssenceText)
    {
        FString LifeEssenceString = FString::Printf(TEXT("%d"), NewLifeEssence); // 정수 형태로 변환
        LifeEssenceText->SetText(FText::FromString(LifeEssenceString));
    }
}

void URSDunShopWidget::PopulateShopItems()
{
    URSGameInstance* GI = Cast<URSGameInstance>(GetGameInstance());

    if (!GI)
    {
        return;
    }

    const TArray<TPair<FName, FItemInfoData*>>& ShopItems = GI->GetCachedDunShopItems();

    for (const auto& ItemPair : ShopItems)
    {
        const FName RowName = ItemPair.Key;
        const FItemInfoData* ItemData = ItemPair.Value;

        if (!ItemData)
        {
            continue;
        }

        if (RSDunShopItemWidgetClass && ItemHorizontalBox)
        {
            URSDunItemWidget* NewItemWidget = CreateWidget<URSDunItemWidget>(this, RSDunShopItemWidgetClass);
            if (NewItemWidget)
            {
                NewItemWidget->SetItemData(*ItemData);
                NewItemWidget->SetItemRowName(RowName);

                ItemHorizontalBox->AddChild(NewItemWidget);
            }
        }
    }
}

void URSDunShopWidget::OnExitClicked()
{
    SetMouseMode(false);

    RemoveFromParent();
}

void URSDunShopWidget::SetMouseMode(bool bEnable)
{
    UWorld* World = GetWorld();

    if (World)
    {
        APlayerController* PlayerController = World->GetFirstPlayerController();

        if (PlayerController)
        {
            // 받고있던 입력 초기화
            PlayerController->FlushPressedKeys();

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