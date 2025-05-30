// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseInventoryComponent.h"
#include "RSDungeonIngredientInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSDungeonIngredientInventoryComponent : public URSBaseInventoryComponent
{
	GENERATED_BODY()
	
public:
	virtual void AddItem(FName ItemKey, int32 Amount = 1) override;

	virtual void RemoveItem(FName ItemKey, int32 Amount = 1) override;

	void DropItem(FName ItemKey);
};
