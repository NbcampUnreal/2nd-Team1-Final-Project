// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RSAttackBufferAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSAttackBufferAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	URSAttackBufferAnimNotifyState();

private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	bool bNextAnimStart;

};
