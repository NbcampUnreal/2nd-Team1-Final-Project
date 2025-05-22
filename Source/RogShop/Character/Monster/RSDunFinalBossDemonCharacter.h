// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunMonsterCharacter.h"
#include "RSDunFinalBossDemonCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSDunFinalBossDemonCharacter : public ARSDunMonsterCharacter
{
	GENERATED_BODY()

public:
	void PlayBaseAttackAnim() override;
	void PlayHitReactAnim() override;
	void PlayDeathAnim() override;

	void PerformAttackTrace() override;
	
};
