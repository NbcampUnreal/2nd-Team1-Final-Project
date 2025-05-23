// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSTileMap.generated.h"

struct FTileRow;
class ARSBaseTile;

UCLASS()
class ROGSHOP_API ARSTileMap : public AActor
{
	GENERATED_BODY()

public:
	ARSTileMap();

	const TArray<TObjectPtr<ARSBaseTile>>& GetTiles() const { return TileActors; }

protected:
	virtual void BeginPlay() override;

private:
	void LoadTiles();
	void SetDefaultSettings();
	void CreateTiles();
	UClass* GetTileClass(const FName& TileKey);

	UFUNCTION(CallInEditor) //임시
	void SaveTiles();

	UFUNCTION(CallInEditor)
	void DeleteTileData();

private:
	static const FString TileMapSaveSlot;

	UPROPERTY(EditAnywhere, Category="TileMap")
	TSubclassOf<ARSBaseTile> DefaultTileType;

	UPROPERTY(EditAnywhere, Category="TileMap")
	int32 DefaultWidth = 3;

	UPROPERTY(EditAnywhere, Category="TileMap")
	int32 DefaultHeight = 3;

	UPROPERTY(EditDefaultsOnly, Category="TileMap")
	TArray<TSubclassOf<ARSBaseTile>> TileTypes;

	UPROPERTY(EditAnywhere, Category="TileMap")
	TObjectPtr<USceneComponent> TileParent;

	UPROPERTY()
	TArray<TObjectPtr<ARSBaseTile>> TileActors;

	UPROPERTY(EditAnywhere, Category="TileMap")
	TArray<FTileRow> TileName2DMap;

	int32 Width, Height;
};
