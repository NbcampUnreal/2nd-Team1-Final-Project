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
	// UFUNCTION(CallInEditor, Category="TileMap")
	void CreateTilesWithChildActorComponent();
	void CreateTilesWithSpawnActor();
	
public:
	UPROPERTY(EditAnywhere, Category="TileMap")
	TSubclassOf<ARSBaseTile> DefaultTileType;

	UPROPERTY(EditAnywhere, Category="TileMap")
	int32 Width;

	UPROPERTY(EditAnywhere, Category="TileMap")
	int32 Height;

protected:
	TArray<TWeakObjectPtr<ARSBaseTile>> Tiles;
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TileParent;
};
