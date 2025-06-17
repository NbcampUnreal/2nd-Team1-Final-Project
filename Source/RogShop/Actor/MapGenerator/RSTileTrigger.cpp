// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTileTrigger.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "RSDungeonGameModeBase.h"
#include "RSSpawnManager.h"


ARSTileTrigger::ARSTileTrigger()
{
	OnActorBeginOverlap.AddDynamic(this, &ARSTileTrigger::OnOverlapBegin);
}


void ARSTileTrigger::BeginPlay()
{
	Super::BeginPlay();
	InitTileCoord();
}

void ARSTileTrigger::InitTileCoord()
{
	FVector Location = GetActorLocation();
	int32 X = FMath::RoundToInt(Location.X / 4000.f);
	int32 Y = FMath::RoundToInt(Location.Y / 4000.f);
	TileCoord = FIntPoint(X, Y);
}

void ARSTileTrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (bMonsterSpawned || !OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	if (TileCoord == FIntPoint(0, 0))
	{
		return;
	}

	ARSDungeonGameModeBase* GameMode = Cast<ARSDungeonGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!GameMode)
	{
		return;
	}

	URSSpawnManager* SpawnManager = GameMode->GetSpawnManager();
	if (!SpawnManager)
	{
		return;
	}

	SpawnManager->SpawnMonstersAtTile(TileCoord);
	bMonsterSpawned = true;
}
