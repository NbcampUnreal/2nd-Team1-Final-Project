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
class ARSTileBlocker;
class ARSDungeonGroundWeapon;
class ARSDungeonGroundIngredient;
class ARSDungeonGroundRelic;
class ARSDungeonGroundLifeEssence;
class ARSBaseAltar;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawn, ARSDunBaseCharacter*, SpawnCharacter);

/**
 * URSSpawnManager
 * 레벨 내 플레이어, 몬스터, 상점 NPC 스폰을 담당하는 매니저
 */

UCLASS(Blueprintable, EditInlineNew)
class ROGSHOP_API URSSpawnManager : public UObject
{
	GENERATED_BODY()

#pragma region SpawnCharacter
public:

	// 월드와 데이터테이블 초기화
	void Initialize(UWorld* InWorld, UGameInstance* GameInstance,int32 TargetLevelIndex);

	// 플레이어를 시작 위치에 스폰 또는 이동
	void SpawnPlayerAtStartPoint();

	// 보스몬스터 스폰 함수
	UFUNCTION()
	void SpawnBossMonster();

	// 특정 타일에 몬스터 스폰을 위한 함수
	void SpawnMonstersAtTile(FIntPoint TileCoord); 


private:
	UFUNCTION()
	void OnMonsterDiedFromTile(ARSDunBaseCharacter* DiedCharacter); // 타일에서 몬스터가 죽었을 때 호출되는 함수
	TMap<FIntPoint, TArray<AActor*>> BuildTileToTargets(); // 빌드 타일에 대한 타겟 포인트를 생성하는 함수

	// 현재 게임 월드 참조
	UWorld* World;

	// 타일별 몬스터 스폰 관련
	TMap<FIntPoint, TArray<AActor*>> TileToTargets; // 타일 → 몬스터 스폰 위치
	TMap<FIntPoint, int32> AliveMonstersPerTile; // 타일 → 살아있는 몬스터 수

	
	TSet<FIntPoint> SpawnedTiles; // 타일별로 몬스터가 한 번이라도 스폰됐는지	
	TMap<FIntPoint, bool> ClearedTiles; // 타일별로 클리어(= 몬스터 전멸) 여부
#pragma endregion

#pragma region Delegate
public:
	// 적(몬스터, 보스)가 스폰될 때 바인딩될 함수를 연결하는 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnEnemySpawn OnEnemySpawn;
#pragma endregion

#pragma region Wall
	TMap<FIntPoint, TArray<ARSTileBlocker*>> TileToBlockers; // 타일 좌표 → 해당 타일의 블로커 배열 매핑
	void RegisterAllTileBlockers(); // 모든 타일 블로커를 등록하는 함수
#pragma endregion


//포탈 관련 함수
#pragma region Portal

public:
	FVector GetNextStageLocation() const;

	AActor* SpawnBossPortal(const FVector& BossWorldLocation); // 보스 아레나로 가는 포탈을 소환하는 함수

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARSDunBossRoomPortal> DunBossRoomPortalClass; // 보스방 포탈 클래스

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ARSDunBossRoomPortal> DunBossRoomPortalInstance; // 인스턴스

	FIntPoint BossTileCoord; // 보스포탈 생성될 좌표
	FVector BossPotalLoaction;

public:
	// 보스 아레나 위치를 반환하는 함수
	UFUNCTION(BlueprintCallable)
	FVector GetBossArenaLocation() const;

	void SetBossTileCoord(const FVector& BossWorldLocation);

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

	// 제단 생성
	void SpawnAltar();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARSDunLifeEssenceShop> DunLifeEssenceShopClass; // 생명의 정수 상점 클래스

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ARSDunLifeEssenceShop> DunLifeEssenceShopInstance; // 인스턴스

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ARSBaseAltar>> AltarClasses; //제단 클래스

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ARSBaseAltar>> AltarInstance; //제단 인스턴스
#pragma endregion

#pragma region GroundItem
public:
	UFUNCTION()
	void SpawnGroundWeaponAtTransform(FName TargetName, FTransform TargetTransform, bool AddImpulse);

	UFUNCTION()
	void SpawnGroundIngredientAtTransform(FName TargetName, FTransform TargetTransform, int32 Quantity);

	UFUNCTION()
	void SpawnGroundRelicAtTransform(FName TargetName, FTransform TargetTransform);

	UFUNCTION()
	void SpawnGroundLifeEssenceAtTransform(FTransform TargetTransform, int32 Quantity);

private:
	UFUNCTION()
	void SpawnGroundIngredientFromCharacter(ARSDunBaseCharacter* DiedCharacter);

	UFUNCTION()
	void SpawnGroundLifeEssenceFromCharacter(ARSDunBaseCharacter* DiedCharacter);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARSDungeonGroundWeapon> DungeonGroundWeaponClass; // 아이템으로 드랍되는 무기 클래스

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARSDungeonGroundIngredient> DungeonGroundIngredientClass; // 아이템으로 드랍되는 요리 재료 클래스

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARSDungeonGroundRelic> DungeonGroundRelicClass; // 아이템으로 드랍되는 유물 클래스

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARSDungeonGroundLifeEssence> DungeonGroundLifeEssenceClass; // 아이템으로 드랍되는 생명의 정수 클래스
#pragma endregion

#pragma region Unspawned
public:
	TSet<FName> GetUnspawnedWeapons() const;

	void SetUnspawnedWeapons(TArray<FName> NewUnspawnedWeapons);

	void RemoveUnspawnedWeapon(FName RemoveTargetName);

	void ResetUnspawnedWeapons();

	TSet<FName> GetUnspawnedRelics() const;

	void SetUnspawnedRelics(TArray<FName> NewUnspawnedRelics);

	void RemoveUnspawnedRelic(FName RemoveTargetName);

	void ResetUnspawnedRelics();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSet<FName> UnspawnedWeapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSet<FName> UnspawnedRelics;
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