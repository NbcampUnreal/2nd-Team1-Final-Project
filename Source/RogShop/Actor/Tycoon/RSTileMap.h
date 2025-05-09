// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSTileMap.generated.h"

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
	UFUNCTION(CallInEditor, Category="TileMap")
	void CreateTiles();

public:
	UPROPERTY(EditAnywhere, Category="Tile")
	TSubclassOf<ARSBaseTile> TileType;

	UPROPERTY(EditAnywhere, Category="TileMap")
	int32 Width;

	UPROPERTY(EditAnywhere, Category="TileMap")
	int32 Height;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TileParent;
	
	UPROPERTY(EditDefaultsOnly, Category="Tile")
	TSubclassOf<ARSBaseTile> DefaultTileType;
};
