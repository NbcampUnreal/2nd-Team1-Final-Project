// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTileMap.h"

#include "MovieSceneSequenceID.h"
#include "RogShop/UtilDefine.h"
#include "Tile/RSBaseTile.h"

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

	CreateTilesWithSpawnActor(); // 임시
}

void ARSTileMap::CreateTilesWithChildActorComponent()
{
	RS_LOG_F("%d x %d Tile 생성", Width, Height)

	check(DefaultTileType)

	if (TileParent->GetNumChildrenComponents() > 0)
	{
		auto& ChildCompArray = TileParent->GetAttachChildren();
		int Num = ChildCompArray.Num();

		for (int32 i = 0; i < Num; i++)
		{
			auto& Child = ChildCompArray[0];
			if (Child && !Child->IsBeingDestroyed())
			{
				Child->DestroyComponent();
			}
		}
	}

	FVector TileSize = DefaultTileType.GetDefaultObject()->GetTileSize();

	for (int32 i = 0; i < Height; i++)
	{
		for (int32 j = 0; j < Width; j++)
		{
			FName TileName = FName(FString::Printf(TEXT("Tile %d x %d"), i, j));

			//새로운 UChildActorComponent를 생성
			UChildActorComponent* NewChildActorComp = NewObject<UChildActorComponent>(this, TileName);
			NewChildActorComp->SetChildActorClass(DefaultTileType);

			//부모를 설정 후 컴포넌트를 등록
			NewChildActorComp->SetupAttachment(TileParent);
			NewChildActorComp->CreationMethod = EComponentCreationMethod::Instance;
			NewChildActorComp->RegisterComponent();

			FVector Location = FVector::ZeroVector;
			Location.Y += TileSize.Y * j;
			Location.X += TileSize.X * i;

			NewChildActorComp->SetRelativeLocation(Location);
		}
	}
}

void ARSTileMap::CreateTilesWithSpawnActor()
{
	RS_LOG_F("%d x %d Tile 생성", Width, Height)

	check(DefaultTileType)

	//임시, 타일 전체 삭제
	if (Tiles.Num() > 0)
	{
		for (auto& Tile : Tiles)
		{
			if (Tile.Get())
			{
				Tile->Destroy();
			}
		}

		Tiles.Empty();
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
			Tiles.Add(TileActor);
		}
	}

	
}
