// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RSSpawnManager.h"
#include "RSMapGenerator.h"
#include "RSDungeonGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMapFullyLoaded);

// 던전 게임모드 클래스 정의
UCLASS()
class ROGSHOP_API ARSDungeonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
#pragma region 공개 함수
	ARSDungeonGameModeBase(); // 생성자
	virtual void BeginPlay() override; // 게임 시작 시 호출
	UFUNCTION()
	void OnMapReady(); // 맵이 완전히 로드되었을 때 실행되는 콜백
	UFUNCTION()
	void NotifyMapReady(); //MapGenerator가 호출하는 함수
#pragma endregion

#pragma region Delegate
public:
	UPROPERTY(BlueprintAssignable)
	FOnBossDead OnBossDead;

	UPROPERTY(BlueprintAssignable)
	FOnMapFullyLoaded OnMapFullyLoaded;
#pragma endregion

private:
#pragma region 비공개 함수
	void SpawnMap(); // 선택된 맵 타입에 따라 맵 생성
#pragma endregion

protected:
#pragma region 내부 변수
	UPROPERTY()
	TObjectPtr<URSSpawnManager> SpawnManager;// 플레이어 및 몬스터 생성 매니저

	UPROPERTY()
	TObjectPtr<ARSMapGenerator> MapGeneratorInstance; // 현재 활성화된 맵 제너레이터
#pragma endregion

public:
#pragma region 에디터 설정 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	TSubclassOf<ARSMapGenerator> MapGeneratorClass; //맵 생성기 클래스
#pragma endregion

#pragma region Dungeon Info
public:
	int32 GetSeed() const;
	void InitRandSeed();

	int32 GetTileIndex() const;
	void IncrementAtTileIndex();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dungeon Info", meta = (AllowPrivateAccess = "true"))
	int32 Seed;	// 해당 값을 기준으로 맵 생성

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dungeon Info", meta = (AllowPrivateAccess = "true"))
	int32 TileIndex;
#pragma endregion


#pragma region SaveData
public:
	UFUNCTION()
	void SaveDungeonInfo();

private:
	void LoadDungeonInfo();

private:
	const FString DungeonInfoSaveSlotName = TEXT("DungeonInfoSaveSlot");
#pragma endregion
};