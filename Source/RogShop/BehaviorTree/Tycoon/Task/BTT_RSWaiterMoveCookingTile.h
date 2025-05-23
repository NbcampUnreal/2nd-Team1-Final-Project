// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RSWaiterMoveCookingTile.generated.h"

/**
 * 블랙보드의 TargetTile로 설정한 타일로 이동
 */
UCLASS()
class ROGSHOP_API UBTT_RSWaiterMoveCookingTile : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_RSWaiterMoveCookingTile();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
