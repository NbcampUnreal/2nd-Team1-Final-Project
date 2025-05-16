// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunMonsterCharacter.h"
#include "RSDunBossSpiderQueenCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSDunBossSpiderQueenCharacter : public ARSDunMonsterCharacter
{
	GENERATED_BODY()
	
public:
	ARSDunBossSpiderQueenCharacter();

	void PlayBaseAttackAnim() override;
	void PlayHitReactAnim() override;
	void PlayDeathAnim() override;

};
