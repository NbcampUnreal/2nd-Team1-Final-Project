// Fill out your copyright notice in the Description page of Project Settings.
#include "DunItemWidget.h"
#include "RSDunPlayerCharacter.h"
#include "RSPlayerWeaponComponent.h"
#include "Kismet/GameplayStatics.h"

#include "RSBaseWeapon.h"
#include "RSDunPlayerController.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "DunShopWidget.h"

void UDunItemWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BuyBtn)
    {
        BuyBtn->OnClicked.AddDynamic(this, &UDunItemWidget::OnBuyClicked);
    }
}

void UDunItemWidget::SetItemData(const FShopItemData& InItemData)
{
    ItemData = InItemData;

    if (ItemNameText)
    {
        ItemNameText->SetText(InItemData.ItemName);
    }

    if (ItemDesText)
    {
        ItemDesText->SetText(InItemData.Description);
    }

    if (ItemPriceText)
    {
        ItemPriceText->SetText(FText::AsNumber(InItemData.Price));
    }

    if (ItemImage && InItemData.Icon)
    {
        ItemImage->SetBrushFromTexture(InItemData.Icon);
    }
}

void UDunItemWidget::SetParentShop(UDunShopWidget* InShop)
{
    ParentShop = InShop;
}

void UDunItemWidget::OnBuyClicked()
{
    if (BuyItem())
    {
        // Character->SetLifeEssence(ItemData.Price); // �÷��̾� ��� ���� ���� �Լ� �Ǵ� public ���� �ʿ�

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

bool UDunItemWidget::BuyItem()
{
    ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    ARSDunPlayerCharacter* PlayerChar = Cast<ARSDunPlayerCharacter>(PC->GetCharacter());

    if (!PlayerChar)
    {
        UE_LOG(LogTemp, Warning, TEXT("BuyItem - Character or PC is nullptr"));
        return false;
    }

    /*if (Character->GetLifeEssence() - ItemData.Price < 0) // �÷��̾� ��ȭ �������� ���� �Լ� �Ǵ� public ���� �ʿ�
    {
        UE_LOG(LogTemp, Warning, TEXT("BuyItem - More LifeEssence Need"));
        return false;
    }*/

    bool bIsBuy = true;

    float FinalValue = 0.0f;

    switch (ItemData.ItemList)
    {
        case EItemList::Potion: // ü�� ��� ȸ��
        {
            switch (ItemData.Rarity)
            {
                case ERarity::Common: FinalValue = 10.0f; break;
                case ERarity::Rare: FinalValue = 20.0f; break;
                case ERarity::Epic: FinalValue = 30.0f; break;
                case ERarity::Legendary: FinalValue = 40.0f; break;
            }

            PlayerChar->IncreaseHP(FinalValue);
            PC->RSDunMainWidget->UpdateHP();

            break;
        }
        case EItemList::MaxHpRelic:
        {
            switch (ItemData.Rarity)
            {
                case ERarity::Common: FinalValue = 1.0f; break;
                case ERarity::Rare: FinalValue = 2.0f; break;
                case ERarity::Epic: FinalValue = 3.0f; break;
                case ERarity::Legendary: FinalValue = 4.0f; break;
            }

            // Character->SetMaxHP(FinalValue);

            break;
        }
        case EItemList::WalkSpeedRelic: // �ӽ�
        {
            switch (ItemData.Rarity)
            {
                case ERarity::Common: FinalValue = 1.0f; break;
                case ERarity::Rare: FinalValue = 2.0f; break;
                case ERarity::Epic: FinalValue = 3.0f; break;
                case ERarity::Legendary: FinalValue = 4.0f; break;
            }

            // Character->AddWalkSpeed(FinalValue);

            break;
        }
        case EItemList::AttackRelic: // �ӽ�
        {
            switch (ItemData.Rarity)
            {
                case ERarity::Common: FinalValue = 5.0f; break;
                case ERarity::Rare: FinalValue = 10.0f; break;
                case ERarity::Epic: FinalValue = 15.0f; break;
                case ERarity::Legendary: FinalValue = 25.0f; break;
            }

            // Character->AddAtk(FinalValue);

            break;
        }
        case EItemList::Weapon:
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = PlayerChar;
            SpawnParams.Instigator = PlayerChar;

            // ���忡 ���� ���� ������ �ؾ߸� �����͸� �ѱ� �� ����
            ARSBaseWeapon* SpawnedWeapon = GetWorld()->SpawnActor<ARSBaseWeapon>(
                ItemData.WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

            if (SpawnedWeapon)
            {
                URSPlayerWeaponComponent* WeaponComp = PlayerChar->GetRSPlayerWeaponComponent();

                if (WeaponComp)
                {
                    WeaponComp->EquipWeaponToSlot(SpawnedWeapon);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("WeaponComp is Null"));
                    bIsBuy = false;
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("SpawnedWeapon is Null"));
                bIsBuy = false;
            }

            PC->RSDunMainWidget->UpdateWeaponImage(ItemData.Icon);

            break;
        }
        default:
            UE_LOG(LogTemp, Warning, TEXT("Not Define Item"));
            bIsBuy = false;
            break;
    }

    return bIsBuy;
}

