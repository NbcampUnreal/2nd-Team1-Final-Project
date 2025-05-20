// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGenerator.generated.h"

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

	FVector2D GridPos;                // �׸��� ���� ��ǥ (��: (0,0) ~ (2,2))
	EDir Connections = EDir::None;  // �� Ÿ���� ����� ���� ���� (��: ��+�Ʒ� �� ��)
	bool bIsMainPath = false;         // ���� ���(������~������)�� ���ԵǴ� Ÿ������ ����
};


UCLASS()
class ROGSHOP_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnTiles();         // ���� Ÿ���� ���ͷ� ����
	void GenerateMainPath();   // ���������� ��������� ����� �� ��� ����
	void ExpandPathToCoverMinTiles(float MinRatio);  // Ÿ���� ��ü�� ���� ���� �̻� �ǵ��� Ȯ��
	void FindBossRoom();                             // ���� �� ���� ���������� ����
	
	bool IsValidPos(FVector2D Pos) const;                // ��ǥ�� �׸��� �ȿ� �ִ��� Ȯ��
	FVector2D GetNextDirection(FVector2D Current, TArray<FVector2D>& Visited); // ���� ��� ����

public:
	// Ÿ�� �� ����
	UPROPERTY(EditAnywhere, Category="Room Status")
	float TileSize = 4000.0f;
	// ���� �õ�
	UPROPERTY(EditAnywhere, Category = "Room Status")
	int32 Seed = 88888;
	// �׸��� ũ��
	UPROPERTY(EditAnywhere, Category = "Room Status")
	int32 GridSize = 3;

	// �Ӹ�� Ÿ��
	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> LineTile;

	// �� ��� Ÿ��
	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> CornerTile;

	// + ��� Ÿ��
	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> CrossTile;

	// T ��� Ÿ��
	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> TTile;

	// ���ٸ��� ��� Ÿ��
	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> DeadEndTile;

	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> BossRoomTile; // ������ ���� Ÿ��

private:
	// �׸��� �� ������ Ÿ�� ������ �����ϴ� Map
	TMap<FVector2D, FTileData> TileMap;
	// ���� ��Ʈ�� ��ü (�õ� ���)
	FRandomStream RandomStream;
	// ������ ��ġ
	FVector2D BossRoomPos;


};
