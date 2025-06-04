// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"

#include "RSSpawnManager.generated.h"

// 몬스터 데이터 전방 선언
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
	void Initialize(UWorld* InWorld, UGameInstance* GameInstance, TSubclassOf<AActor> ShopNPC);

	// 몬스터들을 타겟포인트 위치에 스폰
	void SpawnMonstersInLevel();

	// 상점 NPC를 스폰
	void SpawnShopNPCInLevel();

	// 플레이어를 시작 위치에 스폰 또는 이동
	void SpawnPlayerAtStartPoint(TSubclassOf<ACharacter> PlayerClass);

#pragma endregion

private:

#pragma region 내부 변수

	// 현재 게임 월드 참조
	UWorld* World;

	// 몬스터 그룹 스폰 데이터 테이블
	UDataTable* MonsterRawTable;

	// 몬스터 상태 데이터 테이블
	UDataTable* MonsterStateTable;

	// 상점 NPC 클래스
	TSubclassOf<AActor> ShopNPCClass;

#pragma endregion

#pragma region BossRoomPortal
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BossRoomPortal", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> DunBossRoomPortalClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BossRoomPortal", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> DunBossRoomPortalInstance;
#pragma endregion

#pragma region StageClear
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StageClear", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> DunNextStagePortalClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StageClear", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> DunNextStagePortalInstance;
#pragma endregion
};