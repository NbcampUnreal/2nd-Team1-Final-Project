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

UBTT_RSWaiterMoveCookingTile::UBTT_RSWaiterMoveCookingTile()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_RSWaiterMoveCookingTile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	const FName TileKey = TEXT("TargetTile");
	ARSCookingTile* Tile = Cast<ARSCookingTile>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TileKey));
	if (Tile == nullptr)
	{
		RS_LOG_C("TargetTile이 설정되있지 않습니다", FColor::Red);
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
	
	const FName TileKey = TEXT("TargetTile");
	const FName CustomerKey = TEXT("TargetCustomer");

	//기본적으로 Interact를 하고 난 후 실행, 여기선 CookingTile과 Interact 후 실행
	//움직임이 완료되면
	ARSTycoonWaiterCharacter* Waiter = Cast<ARSTycoonWaiterCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (!Waiter->HasTarget())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TileKey, nullptr);
		
		AActor* PickupActor = Waiter->GetPickupActor();
		if (PickupActor == nullptr)
		{
			RS_LOG("웨이터 가져가기 취소됨")
			
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(CustomerKey, nullptr);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		ARSBaseFood* Food = Cast<ARSBaseFood>(PickupActor);
		check(Food)

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(CustomerKey, Food->Order.Customer.Get());
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
