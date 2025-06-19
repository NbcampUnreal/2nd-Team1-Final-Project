// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonNeedTileLimit.h"

#include "RSTycoonGameModeBase.h"
#include "RSTycoonPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "RogShop/Actor/Tycoon/Tile/RSBaseTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSCookingTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSDoorTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSIceBoxTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSTableTile.h"

URSTycoonNeedTileLimit::URSTycoonNeedTileLimit()
{
	EventName = TEXT("타일 제한!");
}

bool URSTycoonNeedTileLimit::Condition_Implementation(UWorld* World)
{
	//운영시 문, 화구, 테이블 타일이 한개 이상씩은 있어야함
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(World, ARSTileMap::StaticClass()));
	check(TileMap)
	
	DoorCount = 0, CookingCount = 0, TableCount = 0, IceBoxCount = 0;
	for (auto& Tile : TileMap->GetTiles())
	{
		if (Tile->IsA<ARSDoorTile>())
		{
			DoorCount++;
		}

		if (Tile->IsA<ARSCookingTile>())
		{
			CookingCount++;
		}
		
		if (Tile->IsA<ARSTableTile>())
		{
			TableCount++;
		}

		if (Tile->IsA<ARSIceBoxTile>())
		{
			IceBoxCount++;
		}
	}

	return DoorCount > 0 && CookingCount > 0 && TableCount > 0 && IceBoxCount > 0;
}

void URSTycoonNeedTileLimit::Fail_Implementation(UWorld* World)
{
	Description = TEXT("아래의 타일이 모두 배치되어야 합니다!");
	Description.Append(FString::Printf(TEXT("\n문 : %d / 1"), DoorCount));
	Description.Append(FString::Printf(TEXT("\n화구 : %d / 1"), CookingCount));
	Description.Append(FString::Printf(TEXT("\n테이블 : %d / 1"), TableCount));
	Description.Append(FString::Printf(TEXT("\n상자 : %d / 1"), IceBoxCount));
	
	World->GetFirstPlayerController<ARSTycoonPlayerController>()->SetSaleEnable(false);
}

void URSTycoonNeedTileLimit::Success_Implementation(UWorld* World)
{
	World->GetFirstPlayerController<ARSTycoonPlayerController>()->SetSaleEnable(true);
}
