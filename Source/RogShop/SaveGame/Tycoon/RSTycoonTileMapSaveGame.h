// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RSTycoonTileMapSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FTileRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Tiles;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> YawValues;
};

UCLASS()
class ROGSHOP_API URSTycoonTileMapSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Width;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTileRow> Tile2DMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ChefCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WaiterCount;
};
