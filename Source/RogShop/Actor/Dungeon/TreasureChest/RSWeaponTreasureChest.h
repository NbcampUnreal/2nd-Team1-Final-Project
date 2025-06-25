// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreasureChest/RSBaseTreasureChest.h"
#include "RSWeaponTreasureChest.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSWeaponTreasureChest : public ARSBaseTreasureChest
{
	GENERATED_BODY()
	
public:
	ARSWeaponTreasureChest();

protected:
	virtual void OpenChest() override;
};
