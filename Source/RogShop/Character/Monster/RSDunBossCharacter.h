// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunMonsterCharacter.h"
#include "RSDunBossCharacter.generated.h"

UCLASS()
class ROGSHOP_API ARSDunBossCharacter : public ARSDunMonsterCharacter
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnDeath() override;

	void PlaySpawnAnim() override;

};
