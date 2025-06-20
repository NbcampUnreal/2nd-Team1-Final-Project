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
	void ChangeTileSize(int32 NewWidth, int32 NewHeight, bool bUseGold = false);
	void RotateTile(int32 Index, float YawValue);
	void ResetAllTile();
	int32 GetNeedPrice(int32 NewWidth, int32 NewHeight);

	UFUNCTION(BlueprintCallable)
	void SpawnActorInMap(UClass* ActorClass);

	UFUNCTION()
	void SaveTileMap();

	FVector GetMapCenter();
	FVector GetMapSize();
	const TArray<TObjectPtr<ARSBaseTile>>& GetTiles() const { return TileActors; }
	const TArray<TSubclassOf<ARSBaseTile>>& GetTileTypes() const { return TileClasses; };
	int32 GetWidth() const { return Width; };
	int32 GetHeight() const { return Height; };

protected:
	virtual void BeginPlay() override;

private:
	void LoadTileMap();
	void SetDefaultSettings();
	void CreateTiles();
	ARSBaseTile* CreateTile(const TSubclassOf<ARSBaseTile>& TileClass, int32 Row, int32 Column);
	void CreateWalls(const FVector& Location, int32 Row, int32 Column);
	void CreateWall(TSubclassOf<AActor> WallActorClass, const FVector& Location, float Yaw);
	void ActiveNPC();

	UFUNCTION(CallInEditor)
	void DeleteTileData();

	TSubclassOf<ARSBaseTile> GetTileClass(const FName& TileKey);
	FVector GetRandomLocationInMap();

private:
	UPROPERTY(EditDefaultsOnly, Category="TileMap")
	TSubclassOf<ARSBaseTile> DefaultTileType;

	UPROPERTY(EditDefaultsOnly, Category="TileMap")
	int32 DefaultWidth = 3;

	UPROPERTY(EditDefaultsOnly, Category="TileMap")
	int32 DefaultHeight = 3;
	
	UPROPERTY(EditDefaultsOnly, Category="TileMap")
	int32 TilePrice = 100; //타일 증축시 타일 당 들어가는 돈
	
	UPROPERTY(EditDefaultsOnly, Category="TileMap")
	TArray<TSubclassOf<ARSBaseTile>> TileClasses;

	UPROPERTY(EditAnywhere, Category="TileMap")
	TObjectPtr<USceneComponent> TileParent;

	//에디터에서 편집 가능하게 EditAnywhere
	UPROPERTY(EditAnywhere, Category="TileMap")
	TArray<FTileRow> TileName2DMap;

	UPROPERTY(EditDefaultsOnly, Category="TileMap")
	TSubclassOf<AActor> WallClass;
	
	UPROPERTY(EditDefaultsOnly, Category="TileMap")
	TSubclassOf<AActor> PillarClass;

	UPROPERTY(EditDefaultsOnly, Category="NPC")
	TSubclassOf<ARSTycoonWaiterCharacter> WaiterType;

	UPROPERTY(EditDefaultsOnly, Category="NPC")
	TSubclassOf<ARSTycoonChefCharacter> ChefType;

	UPROPERTY()
	TObjectPtr<ANavMeshBoundsVolume> NavVolume;

	UPROPERTY()
	TArray<TObjectPtr<ARSBaseTile>> TileActors; //실제 타일들 Actor

	int32 Width, Height;
};
