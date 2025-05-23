// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RSWaiterMoveTable.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Tile/RSTableTile.h"
#include "Tycoon/NPC/RSTycoonCustomerCharacter.h"
#include "Tycoon/NPC/RSTycoonWaiterCharacter.h"

UBTT_RSWaiterMoveTable::UBTT_RSWaiterMoveTable()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_RSWaiterMoveTable::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	const FName CustomerKey = TEXT("TargetCustomer");
	ARSTycoonCustomerCharacter* Customer = Cast<ARSTycoonCustomerCharacter>(
		OwnerComp.GetBlackboardComponent()->GetValueAsObject(CustomerKey));
	if (Customer == nullptr)
	{
		RS_LOG_C("TargetCustomer가 설정되있지 않습니다", FColor::Red)
		return EBTNodeResult::Failed;
	}

	ACharacter* Character = OwnerComp.GetAIOwner()->GetCharacter();
	check(Character)

	ARSTycoonWaiterCharacter* Owner = Cast<ARSTycoonWaiterCharacter>(Character);
	check(Owner)

	Owner->MoveToTarget(Customer->GetActorLocation(), Customer->GetSittingTable());

	return EBTNodeResult::Type::InProgress;
}

void UBTT_RSWaiterMoveTable::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//움직임이 완료되면
	ARSTycoonWaiterCharacter* Waiter = Cast<ARSTycoonWaiterCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (!Waiter->IsMoving())
	{
		//임시, 테이블 로직 수정해야함
		// const FName CustomerKey = TEXT("TargetCustomer");
		/// OwnerComp.GetBlackboardComponent()->SetValueAsObject(CustomerKey, nullptr);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
