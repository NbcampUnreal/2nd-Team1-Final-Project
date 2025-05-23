// Fill out your copyright notice in the Description page of Project Settings.

#include "RSDunMainWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "RSDunPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DungeonItemData.h"
#include "RSDataSubsystem.h"

void URSDunMainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UpdateHP();
    UpdateMaxHP();
}

void URSDunMainWidget::UpdateWeaponSlot(uint8 SlotIndex, FName WeaponKey)
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

                UE_LOG(LogTemp, Warning, TEXT("SlotIndex: %d"), SlotIndex);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Not FoundData"));
            }
        }
    }
}

void URSDunMainWidget::UpdateHP()
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

void URSDunMainWidget::UpdateMaxHP()
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
