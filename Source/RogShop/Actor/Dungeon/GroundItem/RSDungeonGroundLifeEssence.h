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

	virtual void Tick(float DeltaTime) override;

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

public:
	void SetQuantity(int32 NewQuantity);

private:
	int32 Quantity;

private:
	// 추적 대상
	TObjectPtr<ARSDunPlayerCharacter> TargetActor;
	
	// 추적 속도
	float CharacterFollowSpeed;

	// 상호작용 타이머 및 딜레이 타임
	FTimerHandle InteractDelayTimer;

	float InteractDelayTime;
};
