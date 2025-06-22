// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_RSCanServing.h"

#include "AIController.h"
#include "RSTycoonGameModeBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
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
	const FName TileMapKey = TEXT("TileMap");
	ARSTileMap* TileMap = Cast<ARSTileMap>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TileMapKey));
	TArray<ARSBaseTile*> Tiles = TileMap->GetTiles();

	//== 테스트 ==
	// TArray<AActor*> Tiles;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSCookingTile::StaticClass(), Tiles);

	const FName TileKey = TEXT("TargetTile");
	ARSTycoonWaiterCharacter* Waiter = Cast<ARSTycoonWaiterCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	for (AActor* Tile : Tiles)
	{
		if (ARSCookingTile* CookingTile = Cast<ARSCookingTile>(Tile))
		{
			if (CookingTile->GetState() == ECookingState::Finish)
			{
				//이미 다른 웨이터가 타일이 배치가 되있으면 제외
				if (CheckAssignOtherWaiter(CookingTile))
				{
					continue;
				}
				
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(TileKey, CookingTile);
				return true;
			}
		}
	}

	return false;
}


bool UBTD_RSCanServing::CheckAssignOtherWaiter(ARSCookingTile* Target) const
{
	const FName TileKey = TEXT("TargetTile");
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	for (ARSTycoonNPC* NPC : GameMode->GetNPCs())
	{
		if (ARSTycoonWaiterCharacter* Waiter = Cast<ARSTycoonWaiterCharacter>(NPC))
		{
			AAIController* WaiterController = Cast<AAIController>(Waiter->GetController());
			ARSCookingTile* OtherWaiterTargetTile = Cast<ARSCookingTile>(
				WaiterController->GetBlackboardComponent()->GetValueAsObject(TileKey));

			if (OtherWaiterTargetTile == Target)
			{
				return true;
			}
		}
	}

	return false;
}
