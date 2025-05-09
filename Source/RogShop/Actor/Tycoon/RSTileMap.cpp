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
}

void ARSTileMap::CreateTiles()
{
	RS_LOG_F("%d x %d Tile 생성", Width, Height)

	check(DefaultTileType)
	check(TileType)

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
			NewChildActorComp->SetChildActorClass(TileType);

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
