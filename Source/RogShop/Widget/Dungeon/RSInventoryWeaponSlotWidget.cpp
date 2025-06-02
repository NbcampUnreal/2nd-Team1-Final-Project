// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInventoryWeaponSlotWidget.h"

#include "RSDunPlayerController.h"
#include "RSDunPlayerCharacter.h"
#include "RSPlayerWeaponComponent.h"
#include "RSDataSubsystem.h"
#include "DungeonItemData.h"

#include "Components/Image.h"

void URSInventoryWeaponSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

    ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();

    if (RSDunPlayerController)
    {
        RSDunPlayerController->OnWeaponSlotChange.AddDynamic(this, &URSInventoryWeaponSlotWidget::UpdateWeaponSlot);
    }
}

FReply URSInventoryWeaponSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // 좌클릭 키다운
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        if (WeaponSlot1 && WeaponSlot1->IsHovered())
        {
            HeldSlotType = EWeaponSlotType::Slot1;
        }
        else if (WeaponSlot2 && WeaponSlot2->IsHovered())
        {
            HeldSlotType = EWeaponSlotType::Slot2;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid slot on mouse down"));
            return FReply::Unhandled();
        }

        GetWorld()->GetTimerManager().SetTimer(HoldTimerHandle, this, &URSInventoryWeaponSlotWidget::HandleLongPress, HoldThreshold, false);

        return FReply::Handled();
    }

    return FReply::Unhandled();
}

FReply URSInventoryWeaponSlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);
        HeldSlotType = EWeaponSlotType::None;

        return FReply::Handled();
    }

    return FReply::Unhandled();
}

void URSInventoryWeaponSlotWidget::HandleLongPress()
{
    UE_LOG(LogTemp, Warning, TEXT("Long Press Detected on URSInventoryWeaponSlotWidget Index: %d"), (int32)HeldSlotType);

    // 추가 작업 필요
    ARSDunPlayerCharacter* DunPlayerChar = GetOwningPlayerPawn<ARSDunPlayerCharacter>();
    if (!DunPlayerChar)
    {
        return;
    }

    URSPlayerWeaponComponent* PlayerWeaponComp = DunPlayerChar->GetRSPlayerWeaponComponent();
    if (PlayerWeaponComp)
    {
        PlayerWeaponComp->DropWeaponToSlot(static_cast<EWeaponSlot>(HeldSlotType));
    }
}

void URSInventoryWeaponSlotWidget::UpdateWeaponSlot(int8 WeaponSlotIndex, FName WeaponKey)
{
    if (URSDataSubsystem* DataSubsystem = GetGameInstance()->GetSubsystem<URSDataSubsystem>())
    {
        if (DataSubsystem->Weapon)
        {
            const FDungeonItemData* FoundData = DataSubsystem->Weapon->FindRow<FDungeonItemData>(
                WeaponKey,
                TEXT("Weapon Data Lookup") // 디버깅용 Context
            );

            if (FoundData)
            {
                if (WeaponSlot1->Brush.GetResourceObject() == nullptr)
                {
                    WeaponSlot1->SetBrushFromTexture(FoundData->ItemIcon);
                }
                else if (WeaponSlot2->Brush.GetResourceObject() == nullptr)
                {
                    WeaponSlot2->SetBrushFromTexture(FoundData->ItemIcon);
                }
                else
                {
                    if (WeaponSlotIndex == 0 || WeaponSlotIndex == 1)
                    {
                        WeaponSlot1->SetBrushFromTexture(FoundData->ItemIcon);
                    }
                    else
                    {
                        WeaponSlot2->SetBrushFromTexture(FoundData->ItemIcon);
                    }
                }
            }
            else
            {
                if (WeaponSlotIndex == 1)
                {
                    WeaponSlot1->SetBrushFromTexture(nullptr);
                }
                else if (WeaponSlotIndex == 2)
                {
                    WeaponSlot2->SetBrushFromTexture(nullptr);
                }
            }
        }
    }
}