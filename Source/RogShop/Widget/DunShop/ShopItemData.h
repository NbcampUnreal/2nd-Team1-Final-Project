#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // ������ ���̺��
#include "RSBaseWeapon.h"
#include "ShopItemData.generated.h"

UENUM(BlueprintType)
enum class ERarity : uint8
{
    Common,
    Rare,
    Epic,
    Legendary
};

UENUM(BlueprintType)
enum class EItemList : uint8
{
    None,
    Potion,
    MaxHpRelic,
    WalkSpeedRelic,
    AttackRelic,
    AttackSpeedRelic,
    Weapon,
};

USTRUCT(BlueprintType)
struct ROGSHOP_API FShopItemData : public FTableRowBase
{
    GENERATED_BODY()

public:

    // ���� ���� ó����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemID = NAME_None;

    // �����ִ� �ؽ�Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText ItemName = FText::FromString(TEXT(""));

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText Description = FText::FromString(TEXT(""));

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Price = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* Icon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    ERarity Rarity = ERarity::Common;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemList ItemList = EItemList::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSubclassOf<ARSBaseWeapon> WeaponClass;
};
