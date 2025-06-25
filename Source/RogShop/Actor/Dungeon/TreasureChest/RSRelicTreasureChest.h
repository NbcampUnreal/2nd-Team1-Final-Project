// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTreasureChest.h"
#include "RSRelicTreasureChest.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSRelicTreasureChest : public ARSBaseTreasureChest
{
	GENERATED_BODY()
	
public:
	ARSRelicTreasureChest();

protected:
	virtual void OpenChest() override;
};
