// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseInventoryComponent.h"
#include "RSDungeonInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSDungeonInventoryComponent : public URSBaseInventoryComponent
{
	GENERATED_BODY()
	
public:
	void DropItem(FName ItemKey);
};
