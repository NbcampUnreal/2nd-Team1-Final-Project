// Fill out your copyright notice in the Description page of Project Settings.


#include "RSPlayerStatusWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "RSDunPlayerController.h"
#include "RSDunPlayerCharacter.h"

#include "ItemInfoData.h"
#include "RSDataSubsystem.h"
#include "RSInventorySlotWidget.h"

void URSPlayerStatusWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();

    if (RSDunPlayerController)
    {
        RSDunPlayerController->OnWeaponSlotChange.AddDynamic(this, &URSPlayerStatusWidget::UpdateWeaponSlot);
    }
}

void URSPlayerStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
    
}

void URSPlayerStatusWidget::UpdateWeaponSlot(int8 WeaponSlotIndex, FName WeaponKey)
{
    if (URSDataSubsystem* DataSubsystem = GetGameInstance()->GetSubsystem<URSDataSubsystem>())
    {
        UDataTable* WeaponInfoDatable = DataSubsystem->WeaponInfo;
        if (WeaponInfoDatable)
        {
            const FItemInfoData* FoundItemInfoDataRow = WeaponInfoDatable->FindRow<FItemInfoData>(WeaponKey, TEXT("Get ItemInfoData"));

            if (FoundItemInfoDataRow)
            {
                UObject* CurItemIcon = FoundItemInfoDataRow->ItemIcon;

                if (WeaponSlot1->GetItemIcon() == nullptr)
                {
                    WeaponSlot1->SetSlotItemInfo(WeaponKey, CurItemIcon, "");
                }
                else if (WeaponSlot2->GetItemIcon() == nullptr)
                {
                    WeaponSlot2->SetSlotItemInfo(WeaponKey, CurItemIcon, "");
                }
                else
                {
                    if (WeaponSlotIndex == 0 || WeaponSlotIndex == 1)
                    {
                        WeaponSlot1->SetSlotItemInfo(WeaponKey, CurItemIcon, "");
                    }
                    else
                    {
                        WeaponSlot2->SetSlotItemInfo(WeaponKey, CurItemIcon, "");
                    }
                }
            }
            else
            {
                if (WeaponSlotIndex == 0 || WeaponSlotIndex == 1)
                {
                    WeaponSlot1->SetSlotItemInfo(NAME_None, nullptr, "");
                }
                else
                {
                    WeaponSlot2->SetSlotItemInfo(NAME_None, nullptr, "");
                }
            }
        }
    }
}