// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunMonsterCharacter.h"
#include "RSDunAnubisCharacter.generated.h"

UCLASS()
class ROGSHOP_API ARSDunAnubisCharacter : public ARSDunMonsterCharacter
{
	GENERATED_BODY()
	
public:
	ARSDunAnubisCharacter();

	void PlayBaseAttackAnim() override;
	void PlayHitReactAnim() override;
	void PlayDeathAnim() override;

	void PerformAttackTrace() override;
};
