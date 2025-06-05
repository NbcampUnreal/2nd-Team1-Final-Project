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
	float GetMoveSpeed() const;
	void ChangeMoveSpeed(float Amount);

	float GetMaxHP() const;
	virtual void ChangeMaxHP(float Amount);
	virtual void IncreaseMaxHP(float Amount);
	virtual void DecreaseMaxHP(float Amount);

	float GetHP() const;
	virtual void ChangeHP(float Amount);
	virtual void IncreaseHP(float Amount);
	virtual void DecreaseHP(float Amount);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Status", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Status", meta = (AllowPrivateAccess = "true"))
	float MaxHP;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "Status", meta = (AllowPrivateAccess = "true"))
	float HP;

// 상태 관련
public:
	virtual void OnDeath();

	bool GetIsDead();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "State", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

// 애니메이션 몽타주 관련
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage")
	TObjectPtr<UAnimMontage> DeathMontage; // 사망
};
