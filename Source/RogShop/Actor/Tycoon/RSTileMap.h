// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSTileMap.generated.h"

class ARSTycoonChefCharacter;
class ARSTycoonWaiterCharacter;
class ANavMeshBoundsVolume;
struct FTileRow;
class ARSBaseTile;

UCLASS()
class ROGSHOP_API ARSTileMap : public AActor
{
	GENERATED_BODY()

public:
	ARSTileMap();

	void ChangeTile(int32 Index, FName TileKey);
	void SaveTileMap();
	void ChangeTileSize(int32 NewWidth, int32 NewHeight);

	UFUNCTION(BlueprintCallable)
	void SpawnActorInMap(UClass* ActorClass);
	
	FVector GetMapCenter();
	FVector GetMapSize();
	const TArray<TObjectPtr<ARSBaseTile>>& GetTiles() const { return TileActors; }
	const TArray<TSubclassOf<ARSBaseTile>>& GetTileTypes() const { return TileTypes; };
	int32 GetWidth() const { return Width; };
	int32 GetHeight() const { return Height; };

protected:
	virtual void BeginPlay() override;
	
private:
	void LoadTileMap();
	void SetDefaultSettings();
	void CreateTiles();
	TSubclassOf<ARSBaseTile> GetTileClass(const FName& TileKey);
	ARSBaseTile* CreateTile(const TSubclassOf<ARSBaseTile>& TileClass, int32 Row, int32 Column);
	void ActiveNPC();
	
	UFUNCTION(CallInEditor)
	void DeleteTileData();

private:
	static const FString TileMapSaveSlot;

	UPROPERTY(EditDefaultsOnly, Category="TileMap")
	TSubclassOf<ARSBaseTile> DefaultTileType;

	UPROPERTY(EditDefaultsOnly, Category="TileMap")
	int32 DefaultWidth = 3;

	UPROPERTY(EditDefaultsOnly, Category="TileMap")
	int32 DefaultHeight = 3;

	UPROPERTY(EditDefaultsOnly, Category="TileMap")
	TArray<TSubclassOf<ARSBaseTile>> TileTypes;

	UPROPERTY(EditAnywhere, Category="TileMap")
	TObjectPtr<USceneComponent> TileParent;

	//에디터에서 편집 가능하게 EditAnywhere
	UPROPERTY(EditAnywhere, Category="TileMap")
	TArray<FTileRow> TileName2DMap;

	UPROPERTY(EditDefaultsOnly, Category="NPC")
	TSubclassOf<ARSTycoonWaiterCharacter> WaiterType;
	
	UPROPERTY(EditDefaultsOnly, Category="NPC")
	TSubclassOf<ARSTycoonChefCharacter> ChefType;
	
	UPROPERTY()
	TObjectPtr<ANavMeshBoundsVolume> NavVolume;

	UPROPERTY()
	TArray<TObjectPtr<ARSBaseTile>> TileActors;

	int32 Width, Height;
};