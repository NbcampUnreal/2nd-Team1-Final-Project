// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsAtkRange.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API UBTD_IsAtkRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_IsAtkRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	static const float atkRange;
};
