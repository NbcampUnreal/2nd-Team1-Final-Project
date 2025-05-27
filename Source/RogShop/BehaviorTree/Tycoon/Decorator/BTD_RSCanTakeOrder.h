// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_RSCanTakeOrder.generated.h"

class ARSTycoonCustomerCharacter;
/**
 * 손님이 주문을 받기를 기다리고 있는가 
 */
UCLASS()
class ROGSHOP_API UBTD_RSCanTakeOrder : public UBTDecorator
{
	GENERATED_BODY()

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	bool CheckAssignOtherWaiter(const FName& Key, ARSTycoonCustomerCharacter* Target) const;
};
