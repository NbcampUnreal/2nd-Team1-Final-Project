// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunBaseCharacter.h"
#include "RSDunMonsterCharacter.generated.h"

class ARSMonsterAIController;
class UMeleeAttackBoxComponent;

UCLASS()
class ROGSHOP_API ARSDunMonsterCharacter : public ARSDunBaseCharacter
{
	GENERATED_BODY()

public:
	ARSDunMonsterCharacter();

	// �ִϸ��̼� ���� �Լ�
	virtual void PlayBaseAttackAnim();
	virtual void PlayHitReactAnim();
	virtual void PlayDeathAnim();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UMeleeAttackBoxComponent> MeleeAttackBoxComponent;

protected:
	// �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* BaseAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* DeathMontage;

private:
	TObjectPtr<ARSMonsterAIController> AIController;  // TODO : Ȥ�ó� ĳ���ؼ� �� �� ������ �̸� ������.

};
