// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonSaveGame.h"
#include "ItemSlot.h"
#include "RSDataSubsystem.h"
#include "RSSaveGameSubsystem.h"
#include "RSTycoonTileMapSaveGame.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Tile/RSCookingTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSIceBoxTile.h"

void URSTycoonSaveGame::SetDefault(bool bSetToFile, FString FileName)
{
	Ingredients.Empty();

	//디폴트로 한개의 아이스 박스의 슬롯만큼 있음
	int32 InventoryNum = ARSIceBoxTile::AddInventorySlot;

	if (bSetToFile)
	{
		//배치된 아이스박스의 갯수만큼 인벤토리 크기를 추가해줌
		URSTycoonTileMapSaveGame* TycoonTileMapSaveGame =
			Cast<URSTycoonTileMapSaveGame>(UGameplayStatics::LoadGameFromSlot(FileName, 0));
		if (TycoonTileMapSaveGame)
		{
			for (auto& Row : TycoonTileMapSaveGame->Tile2DMap)
			{
				for (auto& Column : Row.Tiles)
				{
					if (Column == ARSIceBoxTile::GetStaticTileKey())
					{
						InventoryNum += ARSIceBoxTile::AddInventorySlot;
					}
				}
			}
		}
	}
	else
	{
		TArray<AActor*> IceBoxTiles;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSIceBoxTile::StaticClass(), IceBoxTiles);

		InventoryNum *= IceBoxTiles.Num() + 1;
	}

	Ingredients.SetNum(InventoryNum);
	Money = 250;
}
