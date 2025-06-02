// Fill out your copyright notice in the Description page of Project Settings.


#include "RSPlayerStatusWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "RSDunPlayerController.h"
#include "RSDunPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DungeonItemData.h"
#include "RSDataSubsystem.h"

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
                if (WeaponSlotIndex == 0 || WeaponSlotIndex == 1)
                {
                    WeaponSlot1->SetBrushFromTexture(nullptr);
                }
                else
                {
                    WeaponSlot2->SetBrushFromTexture(nullptr);
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
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerChar or HPText Null!"));
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
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerChar or MaxHPText Null!"));
    }
}
