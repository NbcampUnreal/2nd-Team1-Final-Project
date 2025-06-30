// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDungeonGroundItem.h"
#include "RSDungeonGroundRelic.generated.h"

class URSBaseRelic;

UCLASS()
class ROGSHOP_API ARSDungeonGroundRelic : public ARSDungeonGroundItem
{
	GENERATED_BODY()

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;
};
