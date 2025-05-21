// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGenerator.generated.h"

UENUM()
enum class EDir : uint8 //방향표시 열거형(Bitmask형태)
{
	None = 0,        // 방향 없음
	Up = 1 << 0,   // 위쪽
	Down = 1 << 1,   // 아래쪽
	Left = 1 << 2,   // 왼쪽
	Right = 1 << 3    // 오른쪽
};

// 여러 방향을 동시에 사용할 수 있도록 플래그로 지정 | 연산
ENUM_CLASS_FLAGS(EDir)

USTRUCT()
struct FTileData
{
	GENERATED_BODY()

	FVector2D GridPos;                // 그리드 상의 좌표 (예: (0,0) ~ (2,2))
	EDir Connections = EDir::None;  // 이 타일이 연결된 방향 정보 (예: 위+아래 → ㅣ)
	bool bIsMainPath = false;         // 메인 경로(시작점~보스방)에 포함되는 타일인지 여부
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

	void SpawnTiles();         // 실제 타일을 액터로 스폰
	void GenerateMainPath();   // 시작점부터 보스방까지 연결된 주 경로 생성
	void ExpandPathToCoverMinTiles(float MinRatio);  // 타일이 전체의 일정 비율 이상 되도록 확장
	void FindBossRoom();                             // 가장 먼 방을 보스방으로 설정
	
	bool IsValidPos(FVector2D Pos) const;                // 좌표가 그리드 안에 있는지 확인
	FVector2D GetNextDirection(FVector2D Current, TArray<FVector2D>& Visited); // 다음 경로 선택

public:
	// 타일 간 간격
	UPROPERTY(EditAnywhere, Category="Room Status")
	float TileSize = 4000.0f;
	// 랜덤 시드
	UPROPERTY(EditAnywhere, Category = "Room Status")
	int32 Seed = 88888;
	// 그리드 크기
	UPROPERTY(EditAnywhere, Category = "Room Status")
	int32 GridSize = 3;

	// ㅣ모양 타일
	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> LineTile;

	// ㄴ 모양 타일
	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> CornerTile;

	// + 모양 타일
	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> CrossTile;

	// T 모양 타일
	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> TTile;

	// 막다른길 모양 타일
	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> DeadEndTile;

	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> BossRoomTile; // 보스방 전용 타일

private:
	// 그리드 상에 생성된 타일 정보를 저장하는 Map
	TMap<FVector2D, FTileData> TileMap;
	// 랜덤 스트림 객체 (시드 기반)
	FRandomStream RandomStream;
	// 보스방 위치
	FVector2D BossRoomPos;


};
