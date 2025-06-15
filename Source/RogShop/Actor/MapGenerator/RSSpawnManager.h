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
class ARSDunBossRoomPortal;
class ARSDunNextStagePortal;
class ARSDunLifeEssenceShop;
class ARSDunBaseCharacter;
class ARSDungeonGroundIngredient;
class ARSDungeonGroundLifeEssence;

/**
 * URSSpawnManager
 * 레벨 내 플레이어, 몬스터, 상점 NPC 스폰을 담당하는 매니저
 */

UCLASS(Blueprintable, EditInlineNew)
class ROGSHOP_API URSSpawnManager : public UObject
{
	GENERATED_BODY()

public:

#pragma region 공개 함수

	// 월드와 데이터테이블 초기화
	void Initialize(UWorld* InWorld, UGameInstance* GameInstance,int32 TileIndex);

	// 몬스터들을 타겟포인트 위치에 스폰
	void SpawnMonstersInLevel();

	// 플레이어를 시작 위치에 스폰 또는 이동
	void SpawnPlayerAtStartPoint();

	// 보스몬스터 스폰 함수
	void SpawnBossMonster();

#pragma endregion

private:
#pragma region 내부 변수
	// 현재 게임 월드 참조
	UWorld* World;
#pragma endregion

#pragma endregion BossPortal
public:
	FVector GetNextStageLocation() const;

	AActor* SpawnBossPortal(const FVector& BossWorldLocation); // 보스 아레나로 가는 포탈을 소환하는 함수

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARSDunBossRoomPortal> DunBossRoomPortalClass; // 보스방 포탈 클래스

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ARSDunBossRoomPortal> DunBossRoomPortalInstance; // 인스턴스
#pragma endregion

#pragma endregion NextStagePortal
public:
	// 보스 아레나 위치를 반환하는 함수
	UFUNCTION(BlueprintCallable)
	FVector GetBossArenaLocation() const;

	UFUNCTION()
	void SpawnDunNextStagePortal(); // 던전 다음 스테이지로 가는 포탈을 소환하는 함수

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARSDunNextStagePortal> DunNextStagePortalClass; // 다음 스테이지 포탈 클래스

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ARSDunNextStagePortal> DunNextStagePortalInstance; // 인스턴스
#pragma endregion

#pragma region Shop
public:
	// 상점 NPC를 스폰
	void SpawnShopNPCInLevel();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARSDunLifeEssenceShop> DunLifeEssenceShopClass; // 생명의 정수 상점 클래스

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ARSDunLifeEssenceShop> DunLifeEssenceShopInstance; // 인스턴스
#pragma endregion

#pragma region GroundItem
private:
	UFUNCTION()
	void SpawnGroundIngredient(ARSDunBaseCharacter* DiedCharacter);

	UFUNCTION()
	void SpawnGroundLifeEssence(ARSDunBaseCharacter* DiedCharacter);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARSDungeonGroundIngredient> DungeonGroundIngredientClass; // 아이템으로 드랍되는 요리 재료 클래스

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARSDungeonGroundLifeEssence> DungeonGroundLifeEssenceClass; // 아이템으로 드랍되는 생명의 정수 클래스

#pragma endregion

#pragma region DataTableCaching
private:
	// 몬스터 그룹 스폰 데이터 테이블
	UDataTable* MonsterSpawnGroupDataTable;

	// 몬스터 상태 데이터 테이블
	UDataTable* MonsterDataTable;
#pragma endregion

#pragma region StageClear
private:
	int32 LevelIndex; // 현재 레벨 인덱스
#pragma endregion
};