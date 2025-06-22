// Fill out your copyright notice in the Description page of Project Settings.


#include "RSWeaponInventoryWidget.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "RogShop/UtilDefine.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "RSInventorySlotWidget.h"
#include "RSDunPlayerController.h"
#include "RSDunPlayerCharacter.h"
#include "RSPlayerWeaponComponent.h"

void URSWeaponInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

    ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();

    if (RSDunPlayerController)
    {
        RSDunPlayerController->OnWeaponSlotChange.AddDynamic(this, &URSWeaponInventoryWidget::UpdateWeaponSlot);
    }

    if (WeaponSlot1)
    {
        UButton* SlotButton = WeaponSlot1->GetSlotButton();
        if (SlotButton)
        {
            WeaponSlot1->OnSlotClicked.AddDynamic(this, &URSWeaponInventoryWidget::SetClickWeaponName);
            SlotButton->OnPressed.AddDynamic(this, &URSWeaponInventoryWidget::WeaponSlotPress);
            SlotButton->OnReleased.AddDynamic(this, &URSWeaponInventoryWidget::WeaponSlotRelease);
        }
    }

    if (WeaponSlot2)
    {
        UButton* SlotButton = WeaponSlot2->GetSlotButton();
        if (SlotButton)
        {
            WeaponSlot2->OnSlotClicked.AddDynamic(this, &URSWeaponInventoryWidget::SetClickWeaponName);
            SlotButton->OnPressed.AddDynamic(this, &URSWeaponInventoryWidget::WeaponSlotPress);
            SlotButton->OnReleased.AddDynamic(this, &URSWeaponInventoryWidget::WeaponSlotRelease);
        }
    }


    HoldThreshold = 0.5f;
    ClickWeaponName = NAME_None;
}

void URSWeaponInventoryWidget::WeaponSlotPress()
{
    GetWorld()->GetTimerManager().SetTimer(HoldTimerHandle, this, &URSWeaponInventoryWidget::WeaponDrop, HoldThreshold, false);
}

void URSWeaponInventoryWidget::WeaponSlotRelease()
{
    GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);

    ClickWeaponName = NAME_None;
}

void URSWeaponInventoryWidget::WeaponDrop()
{
    ARSDunPlayerCharacter* CurCharacter = GetOwningPlayerPawn<ARSDunPlayerCharacter>();
    if (!CurCharacter)
    {
        return;
    }

    URSPlayerWeaponComponent* WeaponComp = CurCharacter->GetRSPlayerWeaponComponent();
    if (!WeaponComp)
    {
        return;
    }

    EWeaponSlot ClickWeaponSlot = EWeaponSlot::NONE;

    if (WeaponSlot1->GetItemDataTableKey() == ClickWeaponName)
    {
        ClickWeaponSlot = EWeaponSlot::FirstWeaponSlot;
    }
    else
    {
        ClickWeaponSlot = EWeaponSlot::SecondWeaponSlot;
    }

    WeaponComp->DropWeaponToSlot(ClickWeaponSlot);
}

void URSWeaponInventoryWidget::SetClickWeaponName(FName NewClickWeaponName)
{
    ClickWeaponName = NewClickWeaponName;
}

void URSWeaponInventoryWidget::UpdateWeaponSlot(int8 WeaponSlotIndex, FName WeaponKey)
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