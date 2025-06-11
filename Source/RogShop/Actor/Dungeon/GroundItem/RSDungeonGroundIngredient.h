// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDungeonGroundItem.h"
#include "RSDungeonGroundIngredient.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSDungeonGroundIngredient : public ARSDungeonGroundItem
{
	GENERATED_BODY()
	
public:
	ARSDungeonGroundIngredient();

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

public:
	void SetQuantity(int32 NewQuantity);

private:
	int32 Quantity;
};
