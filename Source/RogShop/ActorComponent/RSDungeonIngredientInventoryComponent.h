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
	
protected:
	virtual void BeginPlay() override;

public:
	virtual int32 AddItem(FName ItemKey, int32 Amount = 1) override;

	virtual int32 RemoveItem(FName ItemKey, int32 Amount = 1) override;

	void DropItem(FName ItemKey);

// 세이브/로드
public:
	UFUNCTION()
	virtual void SaveItemData() override;
	virtual void LoadItemData() override;

};
