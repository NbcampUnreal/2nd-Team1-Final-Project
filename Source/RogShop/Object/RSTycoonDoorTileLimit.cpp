// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonDoorTileLimit.h"

#include "RSTycoonPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "RogShop/Actor/Tycoon/Tile/RSBaseTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSDoorTile.h"

URSTycoonDoorTileLimit::URSTycoonDoorTileLimit()
{
	EventName = TEXT("문 회전 제한");
	Description = TEXT("문은 반드시 벽에 있어야해요!\n관리 모드에서 문을 벽에 배치하고, 문을 회전해 봅시다!");
}

bool URSTycoonDoorTileLimit::Condition_Implementation(UWorld* World)
{
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(World, ARSTileMap::StaticClass()));
	check(TileMap);

	int32 Width = TileMap->GetWidth();
	int32 Height = TileMap->GetHeight();

	int32 OutlineRows[] = {0, Height - 1, -1, -1};
	int32 OutlineColumns[] = {-1, -1, 0, Width - 1};
	float YawRotations[] = {180.f, 0.f, 270.f, 90.f};

	auto& Tiles = TileMap->GetTiles();
	for (int32 i = 0; i < Tiles.Num(); i++)
	{
		if (ARSDoorTile* Door = Cast<ARSDoorTile>(Tiles[i]))
		{
			bool bIsOutline = false;
			int32 Row = i / Width;
			int32 Col = i % Width;

			for (int32 j = 0; j < 4; j++)
			{
				//외곽에 있는 타일인지
				if ((OutlineRows[j] >= 0 && OutlineRows[j] == Row ) ||
					(OutlineColumns[j] >= 0 && OutlineColumns[j] == Col))
				{
					//Yaw 값이 올바른지
					if (FMath::IsNearlyEqual(Door->GetActorRotation().Yaw, YawRotations[j]))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

void URSTycoonDoorTileLimit::Fail_Implementation(UWorld* World)
{
	World->GetFirstPlayerController<ARSTycoonPlayerController>()->SetSaleEnable(false);
}

void URSTycoonDoorTileLimit::Success_Implementation(UWorld* World)
{
	World->GetFirstPlayerController<ARSTycoonPlayerController>()->SetSaleEnable(true);
}
