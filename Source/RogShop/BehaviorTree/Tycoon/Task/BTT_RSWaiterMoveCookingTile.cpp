// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RSWaiterMoveCookingTile.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Food/RSBaseFood.h"
#include "RogShop/Actor/Tycoon/Tile/RSCookingTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSTableTile.h"
#include "Tycoon/NPC/RSTycoonCustomerCharacter.h"
#include "Tycoon/NPC/RSTycoonWaiterCharacter.h"


static const FName TileKey = TEXT("TargetTile");
static const FName CustomerKey = TEXT("TargetCustomer");
		
UBTT_RSWaiterMoveCookingTile::UBTT_RSWaiterMoveCookingTile()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_RSWaiterMoveCookingTile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	ARSCookingTile* Tile = Cast<ARSCookingTile>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TileKey));
	if (Tile == nullptr)
	{
		RS_LOG_C("TargetTile이 설정되있지 않습니다", FColor::Red)
		return EBTNodeResult::Failed;
	}

	ACharacter* Character = OwnerComp.GetAIOwner()->GetCharacter();
	check(Character)

	ARSTycoonWaiterCharacter* Owner = Cast<ARSTycoonWaiterCharacter>(Character);
	check(Owner)

	Owner->MoveToTarget(Tile->GetActorLocation(), Tile);

	return EBTNodeResult::Type::InProgress;
}

void UBTT_RSWaiterMoveCookingTile::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//움직임이 완료되면
	ARSTycoonWaiterCharacter* Waiter = Cast<ARSTycoonWaiterCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (!Waiter->IsMoving())
	{
		AActor* PickupActor = Waiter->GetPickupActor();
		check(PickupActor)

		ARSBaseFood* Food = Cast<ARSBaseFood>(PickupActor);
		check(Food)

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TileKey, nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(CustomerKey, Food->Order.Customer.Get());
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
