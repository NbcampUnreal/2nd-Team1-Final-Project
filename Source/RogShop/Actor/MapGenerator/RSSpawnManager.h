// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Engine/TargetPoint.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/Character.h"

#include "Engine/Level.h"
#include "RSSpawnManager.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMonsterSpawnData : public FTableRowBase
{
	GENERATED_BODY()

	// 스폰할 몬스터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> MonsterClass;
};

UCLASS()
class ROGSHOP_API URSSpawnManager : public UObject
{
	GENERATED_BODY()

public:
	// 외부에서 월드와 DataTable을 초기화할 함수
	void Initialize(UWorld* InWorld, UGameInstance* GameInstance, TSubclassOf<AActor> ShopNPC);
	// 주어진 Level에 있는 TargetPoint들을 찾아 몬스터 스폰
	void SpawnMonstersInLevel();
	// 상점NPC스폰을 위한 함수
	void SpawnShopNPCInLevel();
	// Player스폰을 위한 함수
	void SpawnPlayerAtStartPoint(TSubclassOf<ACharacter> PlayerClass);

private:
	// 특정 TargetPoint 위치에 몬스터 하나 스폰
	AActor* SpawnMonsterAtTarget(AActor* TargetPoint);
	// DataTable에서 무작위로 몬스터 클래스 선택
	TSubclassOf<AActor> SelectMonsterClass();
	// 스폰할 월드 객체 참조

private:
	UWorld* World;
	// 몬스터 정보가 담긴 에디터용 데이터 테이블
	UDataTable* MonsterTable;
	// 상점 NPC 클래스
	TSubclassOf<AActor> ShopNPCClass;
};
