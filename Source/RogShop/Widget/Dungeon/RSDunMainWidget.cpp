// Fill out your copyright notice in the Description page of Project Settings.

#include "RSDunMainWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "RSDunPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void URSDunMainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UpdateHP();
    UpdateMaxHP();
}

void URSDunMainWidget::UpdateWeaponSlot(uint8 index, UTexture2D* NewWeaponImage)
{
    if (WeaponSlot1->Brush.GetResourceObject() == nullptr)
    {
        WeaponSlot1->SetBrushFromTexture(NewWeaponImage);
    }
    else if (WeaponSlot2->Brush.GetResourceObject() == nullptr)
    {
        WeaponSlot2->SetBrushFromTexture(NewWeaponImage);
    }
    else
    {
        if (index == 1)
        {
            WeaponSlot1->SetBrushFromTexture(NewWeaponImage);
        }
        else // 2 또는 0, 현재 아무것도 착용 안한 0의 상태일때 2번째 무기가 바뀜
        {
            WeaponSlot2->SetBrushFromTexture(NewWeaponImage);
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
