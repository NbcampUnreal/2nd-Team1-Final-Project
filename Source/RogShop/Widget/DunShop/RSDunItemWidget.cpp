// Fill out your copyright notice in the Description page of Project Settings.
#include "RSDunItemWidget.h"

#include "RSDunShopWidget.h"
#include "RSDunPlayerController.h"
#include "RSDunPlayerCharacter.h"
#include "RSPlayerWeaponComponent.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Kismet/GameplayStatics.h"

void URSDunItemWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BuyBtn)
    {
        BuyBtn->OnClicked.AddDynamic(this, &URSDunItemWidget::OnBuyClicked);
    }
}

void URSDunItemWidget::SetItemData(const FShopItemData& InItemData)
{
    ItemData = InItemData;

    if (ItemNameText)
    {
        ItemNameText->SetText(ItemData.ItemName);
    }

    if (ItemDesText)
    {
        ItemDesText->SetText(ItemData.Description);
    }

    if (ItemPriceText)
    {
        ItemPriceText->SetText(FText::AsNumber(ItemData.Price));
    }

    if (ItemImage && ItemData.Icon)
    {
        ItemImage->SetBrushFromTexture(ItemData.Icon);
    }
}

void URSDunItemWidget::SetParentShop(URSDunShopWidget* InShop)
{
    ParentShop = InShop;
}

void URSDunItemWidget::SetItemRowName(FName RowName)
{
    CurrentRowName = RowName;
}

void URSDunItemWidget::OnBuyClicked()
{
    if (BuyItem())
    {
        // Character->DecreaseLifeEssence(ItemData.Price); // 플레이어 골드 수정 관련 함수 또는 public 변수 필요

        if (ItemPriceText)
        {
            ItemPriceText->SetText(FText::FromString("Sold Out"));
        }

        if (BuyBtn)
        {
            BuyBtn->SetIsEnabled(false);
        }

        if (ParentShop)
        {
            ParentShop->HandleItemPurchase(ItemData.ItemID);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ParentShop is nullptr"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Buy Item"));
    }
}

bool URSDunItemWidget::BuyItem()
{
    ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    ARSDunPlayerCharacter* PlayerChar = Cast<ARSDunPlayerCharacter>(PC->GetCharacter());

    if (!PlayerChar)
    {
        UE_LOG(LogTemp, Warning, TEXT("BuyItem - Character or PC is nullptr"));
        return false;
    }

    /*if (Character->GetLifeEssence() - ItemData.Price < 0) // 플레이어 재화 가져오기 관련 함수 또는 public 변수 필요
    {
        UE_LOG(LogTemp, Warning, TEXT("BuyItem - More LifeEssence Need"));
        return false;
    }*/

    bool bIsBuy = true;

    float FinalValue = 0.0f;

    switch (ItemData.ItemList)
    {
        case EItemList::Potion: // 체력 즉시 회복
        {
            switch (ItemData.Rarity)
            {
                case ERarity::Common: FinalValue = 10.0f; break;
                case ERarity::Rare: FinalValue = 20.0f; break;
                case ERarity::Epic: FinalValue = 30.0f; break;
                case ERarity::Legendary: FinalValue = 40.0f; break;
            }

            PlayerChar->IncreaseHP(FinalValue);

            break;
        }
        case EItemList::Relic:
        {
            URSBaseRelicEffect* Effect = nullptr;

            if (ItemData.EffectClass)
            {
                Effect = NewObject<URSBaseRelicEffect>(this, ItemData.EffectClass);
            }

            if (Effect)
            {
                // ApplyEffect 함수가 URSBaseRelicEffect 자식 클래스에서 구현되어야 함
                Effect->ApplyEffect(PlayerChar, PC, ItemData);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("BuyItem - EffectClass is null"));
                bIsBuy = false;
            }

            break;
        }
        case EItemList::Weapon:
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = PlayerChar;
            SpawnParams.Instigator = PlayerChar;

            // 월드에 무기 액터 생성을 해야만 데이터를 넘길 수 있음
            ARSBaseWeapon* SpawnedWeapon = GetWorld()->SpawnActor<ARSBaseWeapon>(
                ItemData.WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

            URSPlayerWeaponComponent* WeaponComp = PlayerChar->GetRSPlayerWeaponComponent();

            if (SpawnedWeapon && WeaponComp)
            {
                SpawnedWeapon->SetDataTableKey(CurrentRowName);
                WeaponComp->EquipWeaponToSlot(SpawnedWeapon);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("SpawnedWeapon or WeaponComp is Null"));
                bIsBuy = false;
            }

            break;
        }
        default:
            UE_LOG(LogTemp, Warning, TEXT("Not Define Item"));
            bIsBuy = false;
            break;
    }

    return bIsBuy;
}
