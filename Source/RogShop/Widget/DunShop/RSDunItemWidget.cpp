// Fill out your copyright notice in the Description page of Project Settings.
#include "RSDunItemWidget.h"

#include "RSDunShopWidget.h"
#include "RSDunPlayerController.h"
#include "RSDunPlayerCharacter.h"
#include "RSPlayerWeaponComponent.h"
#include "RSBaseWeapon.h"
#include "RSGameInstance.h"
#include "RSBaseRelic.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Kismet/GameplayStatics.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "RogShop/UtilDefine.h"

void URSDunItemWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BuyBtn)
    {
        BuyBtn->OnClicked.RemoveDynamic(this, &URSDunItemWidget::OnBuyClicked);
        BuyBtn->OnClicked.AddDynamic(this, &URSDunItemWidget::OnBuyClicked);
    }
}

void URSDunItemWidget::SetItemData(const FItemInfoData& InItemData)
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

    if (ItemImage && ItemData.ItemIcon)
    {
        ItemImage->SetBrushResourceObject(ItemData.ItemIcon);
    }
}

void URSDunItemWidget::SetItemRowName(FName RowName)
{
    CurrentRowName = RowName;
}

void URSDunItemWidget::OnBuyClicked()
{
    if (BuyItem())
    {
        ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
        ARSDunPlayerCharacter* PlayerChar = Cast<ARSDunPlayerCharacter>(PC->GetCharacter());

        PlayerChar->DecreaseLifeEssence(ItemData.Price);

        if (ItemPriceText)
        {
            ItemPriceText->SetText(FText::FromString("Sold Out"));
        }

        if (BuyBtn)
        {
            BuyBtn->SetIsEnabled(false);
        }

        URSGameInstance* GI = Cast<URSGameInstance>(GetGameInstance());

        if (GI)
        {
            GI->PurchasedItemIDs.Add(CurrentRowName);  // 아이템 구매 후 중복 생성 방지를 위한 해당 아이디 저장
        }
    }
}

bool URSDunItemWidget::BuyItem()
{
    ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    ARSDunPlayerCharacter* PlayerChar = Cast<ARSDunPlayerCharacter>(PC->GetCharacter());

    if (!PlayerChar)
    {
        return false;
    }

    if (PlayerChar->GetLifeEssence() - ItemData.Price < 0)
    {
        RS_LOG_DEBUG("BuyItem - More LifeEssence Need %d", ItemData.Price - PlayerChar->GetLifeEssence());
        return false;
    }

    bool bIsBuy = true;

    switch (ItemData.ItemType)
    {
        case EItemType::Relic:
        {
            FDungeonRelicData* RelicClassData = GetWorld()->GetGameInstance()->GetSubsystem<URSDataSubsystem>()->RelicDetail->FindRow<FDungeonRelicData>(CurrentRowName, TEXT("Get RelicData"));

            if (RelicClassData && RelicClassData->RelicClass)
            {
                URSBaseRelic* RelicInstance = NewObject<URSBaseRelic>(GetTransientPackage(), RelicClassData->RelicClass);

                RelicInstance->ApplyEffect(PlayerChar);
            }
            else
            {
                RS_LOG_DEBUG("RelicClassData is Null");
                bIsBuy = false;
            }

            break;
        }
        case EItemType::Weapon:
        {
            FDungeonWeaponData* WeaponClassData = GetWorld()->GetGameInstance()->GetSubsystem<URSDataSubsystem>()->WeaponDetail->FindRow<FDungeonWeaponData>(CurrentRowName, TEXT("Get WeaponData"));

            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = PlayerChar;
            SpawnParams.Instigator = PlayerChar;

            // 월드에 무기 액터 생성을 해야만 데이터를 넘길 수 있음
            ARSBaseWeapon* SpawnedWeapon = GetWorld()->SpawnActor<ARSBaseWeapon>(
                WeaponClassData->WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

            URSPlayerWeaponComponent* WeaponComp = PlayerChar->GetRSPlayerWeaponComponent();

            if (SpawnedWeapon && WeaponComp)
            {
                SpawnedWeapon->SetNormalAttackDatas(WeaponClassData->NormalAttackData);
                SpawnedWeapon->SetStrongAttackDatas(WeaponClassData->StrongAttackData);

                SpawnedWeapon->SetDataTableKey(CurrentRowName);
                WeaponComp->EquipWeaponToSlot(SpawnedWeapon);
            }
            else
            {
                bIsBuy = false;
            }

            break;
        }
        default:
            RS_LOG_DEBUG("Not Define Item");
            bIsBuy = false;
            break;
    }

    return bIsBuy;
}
