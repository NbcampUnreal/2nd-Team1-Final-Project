// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSlot.generated.h"

USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	FItemSlot() : ItemKey(NAME_None), Quantity(0) {}
};
