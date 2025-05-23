// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RSWaiterMoveTable.generated.h"

/**
 * 블랙보드의 TargetCustomer로 설정된 손님이 앉아 있는 테이블로 이동
 */
UCLASS()
class ROGSHOP_API UBTT_RSWaiterMoveTable : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_RSWaiterMoveTable();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
