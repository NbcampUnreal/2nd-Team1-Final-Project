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
	virtual void OnDeath() override;
	void PlaySpawnAnim() override;
	void UpdateHPUI() override;

	void UpdateBossHP();
	void UpdateBossName();

};
