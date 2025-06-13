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
        RSDunPlayerController->OnHPChange.AddDynamic(this, &URSPlayerStatusWidget::UpdateHP);
        RSDunPlayerController->OnMaxHPChange.AddDynamic(this, &URSPlayerStatusWidget::UpdateMaxHP);
    }
}

void URSPlayerStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
    
    UpdateHP();
    UpdateMaxHP();
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

void URSPlayerStatusWidget::UpdateHP()
{
    ARSDunPlayerCharacter* PlayerChar = Cast<ARSDunPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (PlayerChar && HPText)
    {
        FString HPString = FString::Printf(TEXT("%.0f"), PlayerChar->GetHP()); // 소수점 없는 정수 형태로 변환
        HPText->SetText(FText::FromString(HPString));
    }
}

void URSPlayerStatusWidget::UpdateMaxHP()
{
    ARSDunPlayerCharacter* PlayerChar = Cast<ARSDunPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (PlayerChar && MaxHPText)
    {
        FString MaxHPString = FString::Printf(TEXT("%.0f"), PlayerChar->GetMaxHP()); // 소수점 없는 정수 형태로 변환
        MaxHPText->SetText(FText::FromString(MaxHPString));
    }
}