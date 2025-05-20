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
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

private:
	void CreateTiles();

	void LoadTiles();
	void SaveTiles();
	
public:
	UPROPERTY(EditAnywhere, Category="TileMap")
	TSubclassOf<ARSBaseTile> DefaultTileType;

	UPROPERTY(EditAnywhere, Category="TileMap")
	int32 Width;

	UPROPERTY(EditAnywhere, Category="TileMap")
	int32 Height;

private:
	static const FString TileMapSaveSlot;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TileParent;
	
	UPROPERTY()
	TArray<TWeakObjectPtr<ARSBaseTile>> TileActors;

	UPROPERTY(VisibleAnywhere)
	TArray<FTileRow> TileName2DMap;
};
