// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTreasureChest.h"
#include "RSLifeEssenceTreasureChest.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSLifeEssenceTreasureChest : public ARSBaseTreasureChest
{
	GENERATED_BODY()
	
public:
	ARSLifeEssenceTreasureChest();

protected:
	virtual void OpenChest() override;
};
