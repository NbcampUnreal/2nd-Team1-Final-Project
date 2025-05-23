// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_RSCanServing.h"

#include "AIController.h"
#include "RSTycoonGameModeBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "RogShop/Actor/Tycoon/Tile/RSCookingTile.h"
#include "Tycoon/NPC/RSTycoonWaiterCharacter.h"

bool UBTD_RSCanServing::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)

	//== 실제 게임(RSTileMap 사용) ==
	// TileMap 세팅은 Controller에서 해줄 예정
	// const FName TileMapKey = TEXT("TileMap");
	// ARSTileMap* TileMap = Cast<ARSTileMap>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TileMapKey));
	// TArray<ARSBaseTile*> Tiles = TileMap->GetTiles();

	//== 테스트 ==
	TArray<AActor*> Tiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSCookingTile::StaticClass(), Tiles);

	ARSTycoonWaiterCharacter* Waiter = Cast<ARSTycoonWaiterCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	for (AActor* Tile : Tiles)
	{
		if (ARSCookingTile* CookingTile = Cast<ARSCookingTile>(Tile))
		{
			if (CookingTile->GetState() == ECookingState::Finish)
			{
				const FName TileKey = TEXT("TargetTile");
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(TileKey, CookingTile);
				
				Waiter->MoveToTarget(CookingTile->GetActorLocation(), CookingTile);

				return true;
			}
		}
	}

	return false;
}