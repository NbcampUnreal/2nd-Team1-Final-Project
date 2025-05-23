// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RSWaiterPatrol.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Tile/RSTableTile.h"
#include "Tycoon/NPC/RSTycoonCustomerCharacter.h"
#include "Tycoon/NPC/RSTycoonWaiterCharacter.h"

UBTT_RSWaiterPatrol::UBTT_RSWaiterPatrol()
{
}

EBTNodeResult::Type UBTT_RSWaiterPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//== 테스트용 ==
	TArray<AActor*> Tiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSBaseTile::StaticClass(), Tiles);

	TArray<AActor*> PatrolTiles;
	for (AActor* Tile : Tiles)
	{
		if (Tile->GetClass() == PatrolTileType)
		{
			PatrolTiles.Add(Tile);
		}
	}

	AActor* RandomTile = PatrolTiles[FMath::RandRange(0, PatrolTiles.Num() - 1)];
	OwnerComp.GetAIOwner()->MoveToLocation(RandomTile->GetActorLocation(), 50.f);

	return EBTNodeResult::Type::Succeeded;
}