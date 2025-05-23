// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTileMap.h"

#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/SaveGame/RSTycoonSaveGame.h"
#include "Tile/RSBaseTile.h"
#include "Tile/RSCookingTile.h"
#include "Tile/RSDoorTile.h"
#include "Tile/RSFoodLaboratoryTile.h"
#include "Tile/RSIceBoxTile.h"
#include "Tile/RSTableTile.h"

const FString ARSTileMap::TileMapSaveSlot = TEXT("TileMapSaveSlot");

ARSTileMap::ARSTileMap()
{
	PrimaryActorTick.bCanEverTick = true;

	TileParent = CreateDefaultSubobject<USceneComponent>("TileMap");
	RootComponent = TileParent;
}

void ARSTileMap::BeginPlay()
{
	Super::BeginPlay();

	LoadTiles();
	CreateTiles();
}

void ARSTileMap::LoadTiles()
{
	URSTycoonSaveGame* LoadedGame = Cast<URSTycoonSaveGame>(
		UGameplayStatics::LoadGameFromSlot(TileMapSaveSlot, 0));

	if (LoadedGame)
	{
		RS_LOG_C("타일 로드 성공", FColor::Yellow)
		TileName2DMap = LoadedGame->Tile2DMap;
		Width = LoadedGame->Width;
		Height = LoadedGame->Height;
	}
	else
	{
		RS_LOG_C("타일 로드 실패, 기본 타일로 생성", FColor::Yellow)
		SetDefaultSettings();
	}
}

void ARSTileMap::SetDefaultSettings()
{
	//TileMap 설정
	for (int i = 0; i < DefaultHeight; i++)
	{
		TileName2DMap.Add(FTileRow());
		for (int j = 0; j < DefaultWidth; j++)
		{
			TileName2DMap[i].Tiles.Add(ARSBaseTile::GetStaticTileKey());
		}
	}

	Width = DefaultWidth;
	Height = DefaultHeight;
}

void ARSTileMap::CreateTiles()
{
	check(DefaultTileType)

	RS_LOG_F("%d x %d Tile 생성", Width, Height)

	//배치되어 있는 타일 전체 삭제
	if (TileActors.Num() > 0)
	{
		for (auto& Tile : TileActors)
		{
			if (Tile.Get())
			{
				Tile->Destroy();
			}
		}

		TileActors.Empty();
	}
	
	FVector TileSize = DefaultTileType.GetDefaultObject()->GetTileSize();
	FVector StartLocation = GetActorLocation();
	
	for (int32 i = 0; i < Height; i++)
	{
		for (int32 j = 0; j < Width; j++)
		{
			UClass* TileType = GetTileClass(TileName2DMap[i].Tiles[j]);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ARSBaseTile* TileActor = GetWorld()->SpawnActor<ARSBaseTile>(TileType, SpawnParams);
#if WITH_EDITOR
			TileActor->SetActorLabel(FString::Printf(TEXT("Tile %d x %d"), i, j));
#endif

			FVector Location = StartLocation;
			Location.X += TileSize.X * j;
			Location.Y += TileSize.Y * i;

			TileActor->SetActorLocation(Location);
			TileActors.Add(TileActor);
		}
	}
}

UClass* ARSTileMap::GetTileClass(const FName& TileKey)
{
	for (TSubclassOf<ARSBaseTile> TileType : TileTypes)
	{
		if (TileType->GetDefaultObject<ARSBaseTile>()->GetTileKey() == TileKey)
		{
			return TileType;
		}
	}
	
	//아무것도 매치되지 않음
	checkf(false, TEXT("%s에 매칭되는 타일이 없습니다."), *TileKey.ToString());
	return nullptr;
}

void ARSTileMap::SaveTiles()
{
	// SaveGame 오브젝트 생성
	URSTycoonSaveGame* SaveGameInstance = Cast<URSTycoonSaveGame>(
		UGameplayStatics::CreateSaveGameObject(URSTycoonSaveGame::StaticClass()));

	SaveGameInstance->Tile2DMap = TileName2DMap;
	SaveGameInstance->Width = Width;
	SaveGameInstance->Height = Height;

	// 저장
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TileMapSaveSlot, 0);
}

void ARSTileMap::DeleteTileData()
{
	RS_LOG_C("저장 데이터 삭제", FColor::Orange)
	UGameplayStatics::DeleteGameInSlot(TileMapSaveSlot, 0);
}
