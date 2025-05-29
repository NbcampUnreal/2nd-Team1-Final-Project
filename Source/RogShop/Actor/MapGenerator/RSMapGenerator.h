// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSSpawnManager.h"
#include "Engine/LevelStreamingDynamic.h"
#include "GameFramework/Character.h"
#include "Engine/TargetPoint.h"

#include "Kismet/GameplayStatics.h"
#include "RSMapGenerator.generated.h"

UENUM()
enum class EDir : uint8 //����ǥ�� ������(Bitmask����)
{
	None = 0,        // ���� ����
	Up = 1 << 0,   // ����
	Down = 1 << 1,   // �Ʒ���
	Left = 1 << 2,   // ����
	Right = 1 << 3    // ������
};

// ���� ������ ���ÿ� ����� �� �ֵ��� �÷��׷� ���� | ����
ENUM_CLASS_FLAGS(EDir)

USTRUCT()
struct FTileData
{
	GENERATED_BODY()

	FVector2D GridPos;                // �׸��� ���� ��ǥ
	EDir Connections = EDir::None;  // �� Ÿ���� ����� ���� ���� (ex) ��+�Ʒ� �� ��)
	bool bIsMainPath = false;         // ���� ���(������~������)�� ���ԵǴ� Ÿ������ ����
};


UCLASS()
class ROGSHOP_API ARSMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnTiles();         // ���� Ÿ���� ���ͷ� ����
	void GenerateMainPath();   // ���������� ��������� ����� �� ��� ����
	void ExpandPathToCoverMinTiles(float MinRatio);  // Ÿ���� ��ü�� ���� ���� �̻� �ǵ��� Ȯ��
	void FindBossRoom();                             // ���� �� ���� ���������� ����
	ULevelStreamingDynamic* StreamTile(TSoftObjectPtr<UWorld> LevelToStream, const FVector& Location, const FRotator& Rotation, const FString& UniqueName);
	void ChooseShopTile();

	int32 GetConnectedNeighborCount(FVector2D Pos);
	int32 GetAvailableNeighborCount(FVector2D Pos);
	bool IsValidPos(FVector2D Pos) const;                // ��ǥ�� �׸��� �ȿ� �ִ��� Ȯ��
	FVector2D GetNextDirection(FVector2D Current, TArray<FVector2D>& Visited); // ���� ��� ����
	FVector SpawnBossArenaLevel();

private:
	FVector2D ShopTilePos;
	// Ÿ�� �� ����
	UPROPERTY(EditInstanceOnly, Category="Room Status")
	float TileSize;
	// ���� �õ�
	UPROPERTY(EditInstanceOnly, Category = "Room Status")
	int32 Seed;
	// �׸��� ũ��
	UPROPERTY(EditInstanceOnly, Category = "Room Status")
	int32 GridSize;

	// �Ӹ�� Ÿ��
	UPROPERTY(EditInstanceOnly, Category = "Room Spawning")
	TSoftObjectPtr<UWorld> LineTileLevel;

	// �� ��� Ÿ��
	UPROPERTY(EditInstanceOnly, Category = "Room Spawning")
	TSoftObjectPtr<UWorld> CornerTileLevel;

	// + ��� Ÿ��
	UPROPERTY(EditInstanceOnly, Category = "Room Spawning")
	TSoftObjectPtr<UWorld> CrossTileLevel;

	// T ��� Ÿ��
	UPROPERTY(EditInstanceOnly, Category = "Room Spawning")
	TSoftObjectPtr<UWorld> TTileLevel;

	// ���ٸ��� ��� Ÿ��
	UPROPERTY(EditInstanceOnly, Category = "Room Spawning")
	TSoftObjectPtr<UWorld> DeadEndTileLevel;

	UPROPERTY(EditInstanceOnly, Category = "Room Spawning")
	TSoftObjectPtr<UWorld> BossRoomTileLevel; // ������ ���� Ÿ��
	UPROPERTY(EditInstanceOnly, Category = "Room Spawning")
	TSoftObjectPtr<UWorld> BossArenaLevel; // ������ ���� Ÿ��

	UPROPERTY(EditInstanceOnly, Category = "Spawning")
	TSubclassOf<AActor> ShopNPC;

	UPROPERTY(EditInstanceOnly, Category = "Spawning")
	TSubclassOf<ACharacter> PlayerClass;

	UPROPERTY(EditInstanceOnly, Category = "Spawning")
	UDataTable* MonsterDataTable;


private:
	// �׸��� �� ������ Ÿ�� ������ �����ϴ� Map
	TMap<FVector2D, FTileData> TileMap;
	// ���� ��Ʈ�� ��ü (�õ� ���)
	FRandomStream RandomStream;
	// ������ ��ġ
	FVector2D BossRoomPos;
	URSSpawnManager* SpawnManager;


};
