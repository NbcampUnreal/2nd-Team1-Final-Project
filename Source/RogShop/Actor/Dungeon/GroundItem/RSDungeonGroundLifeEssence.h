// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDungeonGroundItem.h"
#include "RSDungeonGroundLifeEssence.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSDungeonGroundLifeEssence : public ARSDungeonGroundItem
{
	GENERATED_BODY()
	
public:
	ARSDungeonGroundLifeEssence();

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

public:
	void SetQuantity(int32 NewQuantity);

private:
	// 플레이어 캐릭터 추적 타이머 및 딜레이 타임
	FTimerHandle CharacterFollowTimer;

	float CharacterFollowTime;

	// 추적 속도
	float CharacterFollowSpeed;

	// 상호작용 타이머 및 딜레이 타임
	FTimerHandle InteractDelayTimer;

	float InteractDelayTime;

	int32 Quantity;
};
