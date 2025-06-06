// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_RSCanServing.generated.h"

class ARSCookingTile;
/**
 * 
 */
UCLASS()
class ROGSHOP_API UBTD_RSCanServing : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	bool CheckAssignOtherWaiter(ARSCookingTile* Target) const;
};
