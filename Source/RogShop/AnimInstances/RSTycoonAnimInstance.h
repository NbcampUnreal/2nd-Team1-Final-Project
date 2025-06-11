// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RSTycoonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bShouldMove = false;
};
