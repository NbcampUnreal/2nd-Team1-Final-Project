// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTileMap.h"

#include "NavigationSystem.h"
#include "RSTycoonGameModeBase.h"
#include "Components/BrushComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/SaveGame/Tycoon/RSTycoonTileMapSaveGame.h"
#include "Tile/RSBaseTile.h"
#include "Tycoon/NPC/RSTycoonChefCharacter.h"
#include "Tycoon/NPC/RSTycoonNPC.h"
#include "Tycoon/NPC/RSTycoonWaiterCharacter.h"

const FString ARSTileMap::TileMapSaveSlot = TEXT("TileMapSaveSlot");

ARSTileMap::ARSTileMap()
{
	PrimaryActorTick.bCanEverTick = true;

	TileParent = CreateDefaultSubobject<USceneComponent>("TileMap");
	RootComponent = TileParent;
}

void ARSTileMap::ChangeTile(int32 Index, FName TileKey)
{
	if (Index < 0)
	{
		RS_LOG_C("유효하지 않는 타일 Index 입니다", FColor::Red);
		return;
	}

	TileActors[Index]->Destroy();

	TSubclassOf<ARSBaseTile> TileType = GetTileClass(TileKey);
	int32 Row = Index / Width;
	int32 Column = Index % Width;

	TileActors[Index] = CreateTile(TileType, Row, Column);
	TileName2DMap[Row].Tiles[Column] = TileKey;
}

void ARSTileMap::SaveTileMap()
{
	// SaveGame 오브젝트 생성
	URSTycoonTileMapSaveGame* SaveGameInstance = Cast<URSTycoonTileMapSaveGame>(
		UGameplayStatics::CreateSaveGameObject(URSTycoonTileMapSaveGame::StaticClass()));

	SaveGameInstance->Tile2DMap = TileName2DMap;
	SaveGameInstance->Width = Width;
	SaveGameInstance->Height = Height;

	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)

	int32 WaiterCount = 0, ChefCount = 0;
	for (ARSTycoonNPC* NPC : GameMode->GetNPCs())
	{
		if (NPC->IsA<ARSTycoonWaiterCharacter>())
		{
			WaiterCount++;
		}

		if (NPC->IsA<ARSTycoonChefCharacter>())
		{
			ChefCount++;
		}
	}

	SaveGameInstance->WaiterCount = WaiterCount;
	SaveGameInstance->ChefCount = ChefCount;

	// 저장
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TileMapSaveSlot, 0);
}

void ARSTileMap::ChangeTileSize(int32 NewWidth, int32 NewHeight)
{
	Width = NewWidth;
	Height = NewHeight;

	TileName2DMap.SetNum(NewHeight);
	for (FTileRow& Row : TileName2DMap)
	{
		Row.Tiles.SetNum(NewWidth);
		for (int i = 0; i < NewWidth; i++)
		{
			//Array를 확장하면서 생긴 FName이라면 비어있으므로 기본 타일을 넣어줌 
			if (Row.Tiles[i].IsNone())
			{
				Row.Tiles[i] = ARSBaseTile::GetStaticTileKey();
			}
		}
	}

	CreateTiles();
}

void ARSTileMap::SpawnActorInMap(UClass* ActorClass)
{
	FVector HalfTileSize = DefaultTileType->GetDefaultObject<ARSBaseTile>()->GetTileSize() * 0.5f;
	FVector MapSize = GetMapSize();
	FVector RandomLocation = FVector(FMath::FRand()) * MapSize;
	FVector SpawnLocation = GetActorLocation() + RandomLocation - HalfTileSize;
	SpawnLocation.Z = GetActorLocation().Z + 100;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	GetWorld()->SpawnActor(ActorClass, &SpawnLocation, &FRotator::ZeroRotator, SpawnParams);
}

FVector ARSTileMap::GetMapCenter()
{
	FVector Size = GetMapSize();
	FVector Center = GetActorLocation();
	FVector HalfTileSize = DefaultTileType.GetDefaultObject()->GetTileSize() * 0.5f; 

	//Center에서 타일의 중심이 생성되기 때문에 위치를 보정해줘야함
	Center.Y += Size.Y * 0.5f - HalfTileSize.Y;  
	Center.X += Size.X * 0.5f - HalfTileSize.X;

	return Center;
}

FVector ARSTileMap::GetMapSize()
{
	FVector Result = DefaultTileType.GetDefaultObject()->GetTileSize();
	Result.Y *= Width;
	Result.X *= Height;

	return Result;
}

void ARSTileMap::BeginPlay()
{
	Super::BeginPlay();

	LoadTileMap();
	CreateTiles();
	ActiveNPC();
}

void ARSTileMap::LoadTileMap()
{
	URSTycoonTileMapSaveGame* LoadedGame = Cast<URSTycoonTileMapSaveGame>(
		UGameplayStatics::LoadGameFromSlot(TileMapSaveSlot, 0));

	if (LoadedGame)
	{
		RS_LOG_C("타일 로드 성공", FColor::Yellow);
		TileName2DMap = LoadedGame->Tile2DMap;
		Width = LoadedGame->Width;
		Height = LoadedGame->Height;

		for (int i = 0; i < LoadedGame->WaiterCount; i++)
		{
			SpawnActorInMap(WaiterType.Get());
		}

		for (int i = 0; i < LoadedGame->ChefCount; i++)
		{
			SpawnActorInMap(ChefType.Get());
		}
	}
	else
	{
		RS_LOG_C("타일 로드 실패, 기본 타일로 생성", FColor::Yellow);
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

	RS_LOG_F("%d x %d Tile 생성", Width, Height);

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

	for (int32 i = 0; i < Height; i++)
	{
		for (int32 j = 0; j < Width; j++)
		{
			TSubclassOf<ARSBaseTile> TileType = GetTileClass(TileName2DMap[i].Tiles[j]);
			ARSBaseTile* TileActor = CreateTile(TileType, i, j);
			TileActors.Add(TileActor);
		}
	}

	NavVolume = Cast<ANavMeshBoundsVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), ANavMeshBoundsVolume::StaticClass()));
	check(NavVolume)

	NavVolume->SetActorLocation(GetMapCenter());

	FVector NavMeshSize = FVector(TileSize.X * (Height + 1), TileSize.Y * (Width + 1), 100);
	UBrushComponent* BrushComp = NavVolume->GetBrushComponent();

	BrushComp->SetMobility(EComponentMobility::Type::Movable);
	BrushComp->Bounds.BoxExtent = NavMeshSize;
	BrushComp->RegisterComponent();

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSystem)
	{
		NavSystem->OnNavigationBoundsUpdated(NavVolume);
	}
}

TSubclassOf<ARSBaseTile> ARSTileMap::GetTileClass(const FName& TileKey)
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

ARSBaseTile* ARSTileMap::CreateTile(const TSubclassOf<ARSBaseTile>& TileClass, int32 Row, int32 Column)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ARSBaseTile* TileActor = GetWorld()->SpawnActor<ARSBaseTile>(TileClass, SpawnParams);
#if WITH_EDITOR
	TileActor->SetActorLabel(FString::Printf(TEXT("Tile %d x %d"), Row, Column));
#endif

	FVector TileSize = DefaultTileType.GetDefaultObject()->GetTileSize();
	FVector Location = GetActorLocation();
	Location.X += TileSize.X * Row;
	Location.Y += TileSize.Y * Column;

	TileActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	TileActor->SetActorLocation(Location);

	return TileActor;
}

void ARSTileMap::ActiveNPC()
{
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)
	
	for (auto NPC : GameMode->GetNPCs())
	{
		if (ARSTycoonChefCharacter* Chef = Cast<ARSTycoonChefCharacter>(NPC))
		{
			Chef->FindCookingTile();
		}
	}
}


void ARSTileMap::DeleteTileData()
{
	RS_LOG_C("저장 데이터 삭제", FColor::Orange);
	UGameplayStatics::DeleteGameInSlot(TileMapSaveSlot, 0);
}
