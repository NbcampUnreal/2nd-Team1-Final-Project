#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // 데이터 테이블용
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

    // 내부 로직 처리용
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemID;

    // 보여주는 텍스트
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
