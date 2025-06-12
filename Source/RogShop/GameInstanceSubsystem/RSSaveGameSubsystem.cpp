// Fill out your copyright notice in the Description page of Project Settings.


#include "RSSaveGameSubsystem.h"

#include "ItemSlot.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Tile/RSIceBoxTile.h"
#include "Tycoon/RSTycoonSaveGame.h"
#include "Tycoon/RSTycoonTileMapSaveGame.h"

void URSSaveGameSubsystem::AddIngredientDungeonToTycoon(FName IngredientKey, int32 Amount)
{
	URSTycoonSaveGame* TycoonSaveGame = Cast<URSTycoonSaveGame>(
		UGameplayStatics::LoadGameFromSlot(TycoonSaveSlot, 0));
	if (TycoonSaveGame == nullptr)
	{
		RS_LOG_C("TycoonSaveGame 불러오기 실패, 새로운 파일을 생성합니다.", FColor::Red)
		TycoonSaveGame = Cast<URSTycoonSaveGame>(UGameplayStatics::CreateSaveGameObject(URSTycoonSaveGame::StaticClass()));
		
		//디폴트로 한개의 아이스 박스의 슬롯만큼 있음
		int32 InventoryNum = ARSIceBoxTile::AddInventorySlot;
		
		//배치된 아이스박스의 갯수만큼 인벤토리 크기를 추가해줌
		URSTycoonTileMapSaveGame* TycoonTileMapSaveGame = Cast<URSTycoonTileMapSaveGame>(
			UGameplayStatics::LoadGameFromSlot(TycoonTileMapSaveSlot, 0));
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

		TycoonSaveGame->Ingredients.SetNum(InventoryNum);
	}

	TArray<FItemSlot>& Ingredients = TycoonSaveGame->Ingredients;
	int32 NoneIndex = INDEX_NONE;
	
	for (int32 i = 0; i < Ingredients.Num(); i++)
	{
		if (Ingredients[i].ItemKey == IngredientKey)
		{
			Ingredients[i].Quantity += Amount;
			UGameplayStatics::SaveGameToSlot(TycoonSaveGame, TycoonSaveSlot, 0);
			return;
		}

		if (Ingredients[i].ItemKey == FName() || Ingredients[i].ItemKey == TEXT(""))
		{
			NoneIndex = i;
		}
	}

	if (NoneIndex != INDEX_NONE)
	{
		Ingredients[NoneIndex].ItemKey = IngredientKey;
		Ingredients[NoneIndex].Quantity = Amount;
		
		UGameplayStatics::SaveGameToSlot(TycoonSaveGame, TycoonSaveSlot, 0);
	}
	else
	{
		RS_LOG_C("재료를 추가하지 못했습니다.", FColor::Red);
	}
}

void URSSaveGameSubsystem::DeleteAllSaveFile()
{
	DeleteDungeonSaveFile();

	if (UGameplayStatics::DoesSaveGameExist(TycoonSaveSlot, 0))
	{
		UGameplayStatics::DeleteGameInSlot(TycoonSaveSlot, 0);
	}

	if (UGameplayStatics::DoesSaveGameExist(TycoonTileMapSaveSlot, 0))
	{
		UGameplayStatics::DeleteGameInSlot(TycoonTileMapSaveSlot, 0);
	}
}

void URSSaveGameSubsystem::DeleteDungeonSaveFile()
{
	if (UGameplayStatics::DoesSaveGameExist(IngredientInventorySaveSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(IngredientInventorySaveSlotName, 0);
	}

	if (UGameplayStatics::DoesSaveGameExist(WeaponSaveSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(WeaponSaveSlotName, 0);
	}

	if (UGameplayStatics::DoesSaveGameExist(RelicSaveSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(RelicSaveSlotName, 0);
	}

	if (UGameplayStatics::DoesSaveGameExist(StatusSaveSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(StatusSaveSlotName, 0);
	}

	if (UGameplayStatics::DoesSaveGameExist(DungeonInfoSaveSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(DungeonInfoSaveSlotName, 0);
	}
}

bool URSSaveGameSubsystem::DoesDungeonSaveFileExist()
{
	if (!UGameplayStatics::DoesSaveGameExist(IngredientInventorySaveSlotName, 0))
	{
		return false;
	}

	if (!UGameplayStatics::DoesSaveGameExist(WeaponSaveSlotName, 0))
	{
		return false;
	}

	if (!UGameplayStatics::DoesSaveGameExist(RelicSaveSlotName, 0))
	{
		return false;
	}

	if (!UGameplayStatics::DoesSaveGameExist(StatusSaveSlotName, 0))
	{
		return false;
	}

	if (!UGameplayStatics::DoesSaveGameExist(DungeonInfoSaveSlotName, 0))
	{
		return false;
	}

	return true;
}

bool URSSaveGameSubsystem::DoesTycoonSaveFileExist()
{
	if (UGameplayStatics::DoesSaveGameExist(TycoonSaveSlot, 0))
	{
		return true;
	}

	if (UGameplayStatics::DoesSaveGameExist(TycoonTileMapSaveSlot, 0))
	{
		return true;
	}

	return false;
}
