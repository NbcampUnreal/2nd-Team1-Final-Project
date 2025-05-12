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

	// 애니메이션 실행 함수
	virtual void PlayBaseAttackAnim();
	void PlayHitReactAnim();
	void PlayDeathAnim();

protected:
	// 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* BaseAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* DeathMontage;

private:
	TObjectPtr<ARSMonsterAIController> AIController;  // TODO : 혹시나 캐싱해서 쓸 일 생길까봐 미리 만들어둠.

};
