// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "RSDunBossRoomPortal.h" 

#include "RSSpawnManager.generated.h"

// 전방 선언
class UGameInstance;
class ATargetPoint;
class AActor;

/**
 * URSSpawnManager
 * 레벨 내 플레이어, 몬스터, 상점 NPC 스폰을 담당하는 매니저
 */

UCLASS()
class ROGSHOP_API URSSpawnManager : public UObject
{
	GENERATED_BODY()

public:

#pragma region 공개 함수

	// 월드와 데이터테이블 초기화
	void Initialize(UWorld* InWorld, UGameInstance* GameInstance,int32 TileIndex);

	// 몬스터들을 타겟포인트 위치에 스폰
	void SpawnMonstersInLevel();

	// 상점 NPC를 스폰
	void SpawnShopNPCInLevel();

	// 플레이어를 시작 위치에 스폰 또는 이동
	void SpawnPlayerAtStartPoint();

	// 보스몬스터 스폰 함수
	void SpawnBossMonster();

	AActor* SpawnBossPortal(const FVector& BossWorldLocation); // 보스 아레나로 가는 포탈을 소환하는 함수

	// 보스 아레나 위치를 반환하는 함수
	UFUNCTION(BlueprintCallable)
	FVector GetBossArenaLocation() const;
	FVector GetNextStageLocation() const;
	UFUNCTION()
	void SpawnDunNextStagePortal(); // 던전 다음 스테이지로 가는 포탈을 소환하는 함수

#pragma endregion

private:
#pragma region 비공개함수
	void InitalizeDungeonObject();
#pragma endregion

private:

#pragma region 내부 변수

	// 현재 게임 월드 참조
	UWorld* World;

	// 몬스터 그룹 스폰 데이터 테이블
	UDataTable* MonsterRawTable;

	// 몬스터 상태 데이터 테이블
	UDataTable* MonsterStateTable;

	// 오브젝트 데이터 테이블
	UDataTable* DungeonObjectTable;

#pragma endregion
#pragma region StageClear
private:
	int32 LevelIndex; // 현재 레벨 인덱스
#pragma endregion
};