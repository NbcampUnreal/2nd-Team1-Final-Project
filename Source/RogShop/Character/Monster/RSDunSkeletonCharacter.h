// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunMonsterCharacter.h"
#include "RSDunSkeletonCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSDunSkeletonCharacter : public ARSDunMonsterCharacter
{
	GENERATED_BODY()

public:
	ARSDunSkeletonCharacter();

	void PlayBaseAttackAnim() override;
	void PlayHitReactAnim() override;
	void PlayDeathAnim() override;
	
	void PerformAttackTrace() override;
};
