// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/LevelStreamingDynamic.h"
#include "GameFramework/Character.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "RSMapGenerator.generated.h"

UENUM()
enum class EDir : uint8
{
	None = 0,
	Up = 1 << 0,
	Down = 1 << 1,
	Left = 1 << 2,
	Right = 1 << 3
};

ENUM_CLASS_FLAGS(EDir)

USTRUCT()
struct FTileData
{
	GENERATED_BODY()

	FVector2D GridPos;
	EDir Connections = EDir::None;
	bool bIsMainPath = false;
};

UCLASS()
class ROGSHOP_API ARSMapGenerator : public AActor
{
	GENERATED_BODY()

public:
	ARSMapGenerator();

	void SetTileType(int32 IGridSize, float ITileSize, TSoftObjectPtr<UWorld> ILineTileLevel, TSoftObjectPtr<UWorld> ICornerTileLevel, TSoftObjectPtr<UWorld> ICrossTileLevel, TSoftObjectPtr<UWorld> ITTileLevel, TSoftObjectPtr<UWorld> IDeadEndTileLevel, TSoftObjectPtr<UWorld> IBossArenaLevel, TSoftObjectPtr<UWorld> IEnvLevel);
	void StartMapGenerator();


#pragma region 타일로딩상태 검증함수
	UFUNCTION()
	void OnSubLevelLoaded(); // 서브 레벨이 로드되었을 때 호출되는 함수
	bool IsMapFullyLoaded() const; // 맵이 완전히 로드되었는지 확인하는 함수
	void CheckAllTilesLoaded(); // 모든 타일이 로드되었는지 확인하는 함수
#pragma endregion

protected:
	virtual void BeginPlay() override;

#pragma region 맵생성 관련 함수
	void GenerateMainPath(); // 메인 경로 생성함수
	void ExpandPathToCoverMinTiles(float MinRatio); // 최소 타일 비율을 만족하도록 경로 확장 함수
	void FindBossRoom(); // 보스방 위치 찾는 함수
	FVector2D GetNextDirection(FVector2D Current, TArray<FVector2D>& Visited); // 현재 위치에서 이동할 수 있는 다음 위치 선택																																																	
	void ChooseShopTile(); // 상점 타일 위치 선택 함수
	void SpawnTiles(); // 타일 스폰 함수
	FVector SpawnBossArenaLevel(); // 보스 아레나 레벨 스폰함수
	void SpawnEnvLevel(); // 포스트 프로세스 레벨 스폰함수
#pragma endregion

#pragma region 유틸리티 함수
	bool IsValidPos(FVector2D Pos) const; // 선택된 위치가 그리드 안에 있는 유효한 위치인지 확인하는 함수
	int32 GetConnectedNeighborCount(FVector2D Pos); // 현재 위치에서 연결된 이웃 타일의 개수를 반환하는 함수
	int32 GetAvailableNeighborCount(FVector2D Pos); // 현재 위치에서 이동 가능한 이웃 타일의 개수를 반환하는 함수													
	ULevelStreamingDynamic* StreamTile(TSoftObjectPtr<UWorld> LevelToStream, const FVector& Location, const FRotator& Rotation, const FString& UniqueName); // 타일 레벨 스트리밍 함수
#pragma endregion

#pragma region 공개 변수
public:

	// 타일 크기, 그리드 크기
	UPROPERTY(EditAnywhere, Category = "Room Status") // 타일 크기
	float TileSize;

	UPROPERTY(EditAnywhere, Category = "Room Status") // 그리드 크기
	int32 GridSize;

	// 타일 레벨들
	UPROPERTY(EditAnywhere, Category = "Room Spawning") // 직선 모양 타일
	TSoftObjectPtr<UWorld> LineTileLevel;

	UPROPERTY(EditAnywhere, Category = "Room Spawning") // 코너 모양 타일
	TSoftObjectPtr<UWorld> CornerTileLevel;

	UPROPERTY(EditAnywhere, Category = "Room Spawning") // 교차로 모양 타일
	TSoftObjectPtr<UWorld> CrossTileLevel;

	UPROPERTY(EditAnywhere, Category = "Room Spawning") // T모양 타일
	TSoftObjectPtr<UWorld> TTileLevel;

	UPROPERTY(EditAnywhere, Category = "Room Spawning") // 막다른길 타일
	TSoftObjectPtr<UWorld> DeadEndTileLevel;

	UPROPERTY(EditAnywhere, Category = "Room Spawning") // 보스 아레나 타일
	TSoftObjectPtr<UWorld> BossArenaLevel;
	UPROPERTY(EditAnywhere, Category = "Room Spawning") // 포스트 프로세싱 타일
	TSoftObjectPtr<UWorld> EnvLevel;


	UPROPERTY()
	FVector2D ShopTilePos; // 상점 타일 위치

	UPROPERTY()
	TArray<ULevelStreamingDynamic*> SpawnedLevels; // 스트리밍된 레벨 목록
	UPROPERTY()
	FVector BossWorldLocation;

#pragma endregion

#pragma region 비공개 변수
private:
	TMap<FVector2D, FTileData> TileMap;// 타일 맵 데이터
	FRandomStream RandomStream; // 랜덤 스트림
	FVector2D BossRoomPos; // 보스방 위치


	bool bMapGenerationComplete; // 맵 생성 완료 여부
	bool bIsMapLoaded = false; // 맵이 완전히 로드되었는지 여우
#pragma endregion
};