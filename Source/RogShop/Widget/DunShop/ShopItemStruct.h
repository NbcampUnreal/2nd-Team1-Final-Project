#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // ������ ���̺��
#include "ShopItemStruct.generated.h"

UENUM(BlueprintType)
enum class ERarity : uint8
{
    Common,
    Rare,
    Epic,
    Legendary
};

USTRUCT(BlueprintType)
struct ROGSHOP_API FShopItemStruct : public FTableRowBase
{
    GENERATED_BODY()

public:

    // ���� ���� ó����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemID;

    // �����ִ� �ؽ�Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Price;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    ERarity Rarity;
};
