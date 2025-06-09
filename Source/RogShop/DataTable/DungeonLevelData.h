// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DungeonLevelData.generated.h"


/**
 * 
 */
USTRUCT()
struct ROGSHOP_API FDungeonLevelData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UWorld>> EnvLevel;	// 보스 아레나 타일

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UWorld>> LineTileLevel;    // 직선 모양 타일

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UWorld>> CornerTileLevel;  // 코너 모양 타일

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UWorld>> CrossTileLevel;	// 교차로 모양 타일

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UWorld>> TTileLevel;	// T모양 타일

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UWorld>> DeadEndTileLevel;	// 막다른길 타일, 보스방 타일

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UWorld>> BossArenaLevel;	// 보스 아레나 타일
};
