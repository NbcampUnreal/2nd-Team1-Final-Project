// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTileMap.h"

#include "MovieSceneSequenceID.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/SaveGame/RSTycoonSaveGame.h"
#include "Tile/RSBaseTile.h"
#include "Tile/RSTableTile.h"

const FString ARSTileMap::TileMapSaveSlot = TEXT("TileMapSaveSlot");

ARSTileMap::ARSTileMap()
{
	PrimaryActorTick.bCanEverTick = true;

	TileParent = CreateDefaultSubobject<USceneComponent>("TileMap");
	RootComponent = TileParent;
}

void ARSTileMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARSTileMap::BeginPlay()
{
	Super::BeginPlay();

	LoadTiles();
	TileName2DMap[0].Tiles[0] = ARSTableTile::GetTileKey();
	SaveTiles();
	
	// CreateTiles();
}

void ARSTileMap::CreateTiles()
{
	RS_LOG_F("%d x %d Tile 생성", Width, Height)

	check(DefaultTileType)

	//임시, 타일 전체 삭제
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
			ARSBaseTile* TileActor = GetWorld()->SpawnActor<ARSBaseTile>(DefaultTileType);
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

void ARSTileMap::LoadTiles()
{
	URSTycoonSaveGame* LoadedGame = Cast<URSTycoonSaveGame>(
		UGameplayStatics::LoadGameFromSlot(TileMapSaveSlot, 0));
	
	if (LoadedGame)
	{
		RS_LOG_C("타일 로드 성공", FColor::Yellow)
		TileName2DMap = LoadedGame->Tile2DMap;
	}
	else
	{
		for (int i = 0; i < Height; i++)
		{
			TileName2DMap.Add(FTileRow());
			for (int j = 0; j < Width; j++)
			{
				TileName2DMap[i].Tiles.Add(ARSBaseTile::GetTileKey());
			}
		}
	}
}

void ARSTileMap::SaveTiles()
{	
	// SaveGame 오브젝트 생성
	URSTycoonSaveGame* SaveGameInstance = Cast<URSTycoonSaveGame>(
		UGameplayStatics::CreateSaveGameObject(URSTycoonSaveGame::StaticClass()));
	
	SaveGameInstance->Tile2DMap = TileName2DMap;

	// 저장
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TileMapSaveSlot, 0);
}
