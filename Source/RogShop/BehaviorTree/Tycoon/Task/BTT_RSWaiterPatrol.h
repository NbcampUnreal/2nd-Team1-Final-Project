// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RSWaiterPatrol.generated.h"

class ARSBaseTile;
/**
 * 블랙보드의 TargetCustomer로 설정된 손님이 앉아 있는 테이블로 이동
 */
UCLASS()
class ROGSHOP_API UBTT_RSWaiterPatrol : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_RSWaiterPatrol();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARSBaseTile> PatrolTileType;
};
