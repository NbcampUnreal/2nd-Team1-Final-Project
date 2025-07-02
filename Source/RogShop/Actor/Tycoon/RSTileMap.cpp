// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTileMap.h"

#include "NavigationSystem.h"
#include "RSSaveGameSubsystem.h"
#include "RSTycoonGameModeBase.h"
#include "RSTycoonPlayerController.h"
#include "Components/BrushComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/SaveGame/Tycoon/RSTycoonTileMapSaveGame.h"
#include "Tile/RSBaseTile.h"
#include "Tile/RSCookingTile.h"
#include "Tile/RSDoorTile.h"
#include "Tile/RSTableTile.h"
#include "Tycoon/NPC/RSTycoonChefCharacter.h"
#include "Tycoon/NPC/RSTycoonNPC.h"
#include "Tycoon/NPC/RSTycoonWaiterCharacter.h"

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

void ARSTileMap::ChangeTileSize(int32 NewWidth, int32 NewHeight, bool bUseGold)
{
	if (bUseGold)
	{
		ARSTycoonPlayerController* PlayerController =
			Cast<ARSTycoonPlayerController>(
				UGameplayStatics::GetActorOfClass(GetWorld(), ARSTycoonPlayerController::StaticClass()));
		check(PlayerController)

		int32 Price = GetNeedPrice(NewWidth, NewHeight);
		if (PlayerController->GetGold() < Price)
		{
			RS_LOG_C("돈이 부족합니다.", FColor::Red)
			return;
		}

		PlayerController->AddGold(-Price);
	}

	Width = NewWidth;
	Height = NewHeight;

	TileName2DMap.SetNum(NewHeight);
	for (FTileRow& Row : TileName2DMap)
	{
		Row.Tiles.SetNum(NewWidth);
		Row.YawValues.SetNum(NewWidth);
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

void ARSTileMap::RotateTile(int32 Index, float YawValue)
{
	if (Index < 0)
	{
		RS_LOG_C("유효하지 않는 타일 Index 입니다", FColor::Red);
		return;
	}

	int32 Row = Index / Width;
	int32 Column = Index % Width;

	TileActors[Index]->SetActorRotation(FRotator(0, YawValue, 0));
	TileName2DMap[Row].YawValues[Column] = YawValue;
}

void ARSTileMap::ResetAllTile()
{
	for (auto& Tile : TileActors)
	{
		Tile->ResetAll();
	}
}

int32 ARSTileMap::GetNeedPrice(int32 NewWidth, int32 NewHeight)
{
	int32 WidthDistance = NewWidth - Width;
	int32 HeightDistance = NewHeight - Height;
	int32 Price = TilePrice * (WidthDistance + HeightDistance);;

	return Price;
}

void ARSTileMap::SpawnActorInMap(UClass* ActorClass)
{
	FVector SpawnLocation = GetRandomLocationInMap();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor(ActorClass, &SpawnLocation, &FRotator::ZeroRotator, SpawnParams);
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
	FString SlotName = GetWorld()->GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>()->TycoonTileMapSaveSlot;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
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

	GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>()->OnSaveRequested.AddDynamic(this, &ARSTileMap::SaveTileMap);
}

void ARSTileMap::LoadTileMap()
{
	FString SlotName = GetWorld()->GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>()->TycoonTileMapSaveSlot;
	URSTycoonTileMapSaveGame* LoadedGame =
		Cast<URSTycoonTileMapSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

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
			TileName2DMap[i].YawValues.Add(0);
		}
	}

	Width = DefaultWidth;
	Height = DefaultHeight;
}

void ARSTileMap::CreateTiles()
{
	check(DefaultTileType)

	RS_LOG_F("%d x %d Tile 생성", Width, Height);

	//붙어있는 엑터들 (타일, 벽 등) 전체 삭제
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* AttachedActor : AttachedActors)
	{
		AttachedActor->Destroy();
	}

	TileActors.Empty();

	//타일 생성
	for (int32 i = 0; i < Height; i++)
	{
		for (int32 j = 0; j < Width; j++)
		{
			TSubclassOf<ARSBaseTile> TileType = GetTileClass(TileName2DMap[i].Tiles[j]);
			ARSBaseTile* TileActor = CreateTile(TileType, i, j);
			TileActors.Add(TileActor);
		}
	}

	//=== 네비게이션 볼륨 설정 ===
	NavVolume = Cast<ANavMeshBoundsVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), ANavMeshBoundsVolume::StaticClass()));
	check(NavVolume)
	NavVolume->SetActorLocation(GetMapCenter());

	FVector TileSize = DefaultTileType.GetDefaultObject()->GetTileSize();
	FVector NavMeshSize = FVector(TileSize.X * (Height + 1), TileSize.Y * (Width + 1), 20);
	UBrushComponent* BrushComp = NavVolume->GetBrushComponent();

	BrushComp->SetMobility(EComponentMobility::Type::Movable);
	BrushComp->Bounds.BoxExtent = NavMeshSize;
	BrushComp->RegisterComponent();

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSystem)
	{
		NavSystem->OnNavigationBoundsUpdated(NavVolume);
	}

	//NPC, 플레이어 위치를 옮김
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)

	for (auto& NPC : GameMode->GetNPCs())
	{
		FVector Location = GetRandomLocationInMap();
		NPC->SetActorLocation(Location);
	}
	ActiveNPC();

	FVector PlayerRandomLocation = GetRandomLocationInMap();
	GetWorld()->GetFirstPlayerController()->GetCharacter()->SetActorLocation(PlayerRandomLocation);
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
	TileActor->SetActorRelativeRotation(FRotator(0, TileName2DMap[Row].YawValues[Column], 0));

	CreateWalls(Location, Row, Column);

	return TileActor;
}

void ARSTileMap::CreateWalls(const FVector& Location, int32 Row, int32 Column)
{
	//벽, 문 타일이 있다면 설치하지 않음
	//위
	if (Row == Height - 1)
	{
		CreateWall(WallClass, Location, 0.f);
	}
	//아래
	if (Row == 0)
	{
		CreateWall(WallClass, Location, 180.f);
	}
	//왼쪽
	if (Column == 0)
	{
		CreateWall(WallClass, Location, 270.f);
	}
	//오른쪽
	if (Column == Width - 1)
	{
		CreateWall(WallClass, Location, 90.f);
	}

	//기둥
	//왼쪽 위
	if (Row == Height - 1 && Column == 0)
	{
		CreateWall(PillarClass, Location, 0.f);
	}
	//오른쪽 위
	if (Row == Height - 1 && Column == Width - 1)
	{
		CreateWall(PillarClass, Location, 90.f);
	}
	//왼쪽 아래
	if (Row == 0 && Column == 0)
	{
		CreateWall(PillarClass, Location, 270.f);
	}
	//오른쪽 아래
	if (Row == 0 && Column == Width - 1)
	{
		CreateWall(PillarClass, Location, 180.f);
	}
}

void ARSTileMap::CreateWall(TSubclassOf<AActor> WallActorClass, const FVector& Location, float Yaw)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* WallActor = GetWorld()->SpawnActor<AActor>(WallActorClass, SpawnParams);
	WallActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	WallActor->SetActorLocation(Location);
	WallActor->SetActorRelativeRotation(FRotator(0, Yaw, 0));
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

	// FString SlotName = GetWorld()->GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>()->TycoonTileMapSaveSlot;
	FString SlotName = TEXT("TileMapSaveSlot");;
	UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}


TSubclassOf<ARSBaseTile> ARSTileMap::GetTileClass(const FName& TileKey)
{
	for (TSubclassOf<ARSBaseTile> TileType : TileClasses)
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

FVector ARSTileMap::GetRandomLocationInMap()
{
	FVector HalfTileSize = DefaultTileType->GetDefaultObject<ARSBaseTile>()->GetTileSize() * 0.5f;
	FVector MapSize = GetMapSize();
	FVector RandomLocation = FVector(FMath::FRand()) * MapSize;
	FVector SpawnLocation = GetActorLocation() + RandomLocation - HalfTileSize;
	SpawnLocation.Z = GetActorLocation().Z + 100;

	return SpawnLocation;
}
