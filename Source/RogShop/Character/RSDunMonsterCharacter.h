// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunBaseCharacter.h"
#include "RSDunMonsterCharacter.generated.h"

class ARSMonsterAIController;

UCLASS()
class ROGSHOP_API ARSDunMonsterCharacter : public ARSDunBaseCharacter
{
	GENERATED_BODY()

public:
	ARSDunMonsterCharacter();

	// �ִϸ��̼� ���� �Լ�
	virtual void PlayBaseAttackAnim();
	void PlayHitReactAnim();
	void PlayDeathAnim();

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
