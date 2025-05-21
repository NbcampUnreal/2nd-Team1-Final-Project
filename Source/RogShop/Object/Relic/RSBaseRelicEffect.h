// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RSBaseRelicEffect.generated.h"

class ARSDunPlayerCharacter;
class ARSDunPlayerController;
struct FShopItemData;

UCLASS()
class ROGSHOP_API URSBaseRelicEffect : public UObject
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect(ARSDunPlayerCharacter* Player, ARSDunPlayerController* PC, FShopItemData& ItemData) PURE_VIRTUAL(UBaseItemEffect::ApplyEffect, );
};
