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
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;

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

