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

// 해당 엑터의 메시 세팅 및 상호작용에 필요한 변수 세팅
public:
	void InitItemInfo(FName NewDataTableKey, UStaticMesh* NewMesh);

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;
};
