// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Engine/TargetPoint.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/Character.h"
#include "MonsterSpawnGroupData.h"

#include "Engine/Level.h"
#include "RSSpawnManager.generated.h"

/**
 * 
 */
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
	UWorld* World;
	// 몬스터 정보가 담긴 에디터용 데이터 테이블
	UDataTable* MonsterRawTable;
	UDataTable* MonsterStateTable;
	// 상점 NPC 클래스
	TSubclassOf<AActor> ShopNPCClass;
};
