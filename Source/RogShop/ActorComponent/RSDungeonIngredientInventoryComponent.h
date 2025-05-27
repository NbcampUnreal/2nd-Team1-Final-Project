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
	void DropItem(FName ItemKey);
};
