// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RSDungeonItemBase.h"
#include "ItemInfoData.generated.h"

class URSBaseRelic;

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	NONE,
	Common,
	Rare,
	Epic,
	Legendary
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	NONE,
	Weapon,
	Relic,
	CookFood,
	Ingredient,
};

USTRUCT(BlueprintType)
struct ROGSHOP_API FItemInfoData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemRarity ItemRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	// AllowedClasses 설정값이 있어야 에디터 데이터 테이블에서 해당 ItemIcon 값 설정 가능
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "Texture2D, MaterialInstance", DisplayThumbnail = "true"))
	TObjectPtr<UObject> ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> ItemStaticMesh;
};

USTRUCT(BlueprintType)
struct ROGSHOP_API FDungeonWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARSDungeonItemBase> WeaponClass;
};

USTRUCT(BlueprintType)
struct ROGSHOP_API FDungeonRelicData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<URSBaseRelic> RelicClass;
};

