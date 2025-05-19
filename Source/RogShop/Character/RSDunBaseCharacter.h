// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RSDunBaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSDunBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARSDunBaseCharacter();

// 스탯 관련
public:
	float GetMaxHP() const;
	void IncreaseMaxHP(float Amount);
	void DecreaseMaxHP(float Amount);

	float GetHP() const;
	void IncreaseHP(float Amount);
	void DecreaseHP(float Amount);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Stat", meta = (AllowPrivateAccess = "true"))
	float MaxHP;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "Stat", meta = (AllowPrivateAccess = "true"))
	float HP;

// 상태 관련
public:
	bool GetIsDead();
	virtual void OnDeath();
private:
	bool bIsDead;
};
