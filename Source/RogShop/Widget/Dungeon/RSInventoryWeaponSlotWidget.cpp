// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInventoryWeaponSlotWidget.h"

#include "RSDunPlayerController.h"
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

void URSInventoryWeaponSlotWidget::UpdateWeaponSlot(uint8 SlotIndex, FName WeaponKey)
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
                    if (SlotIndex == 0 || SlotIndex == 1)
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
                UE_LOG(LogTemp, Warning, TEXT("Not FoundData"));
            }
        }
    }
}