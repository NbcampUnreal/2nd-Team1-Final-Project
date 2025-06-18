// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RSSpawnManager.h"
#include "RSMapGenerator.h"
#include "RSSpawnManagerAccessor.h"
#include "RSDungeonGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMapFullyLoaded);

// 던전 게임모드 클래스 정의
UCLASS()
class ROGSHOP_API ARSDungeonGameModeBase : public AGameModeBase, public IRSSpawnManagerAccessor
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
	FOnGameReady OnGameReady;

	UPROPERTY(BlueprintAssignable)
	FOnMapFullyLoaded OnMapFullyLoaded;
#pragma endregion

#pragma region MapGenerator
private:
	void SpawnMap(); // 선택된 맵 타입에 따라 맵 생성
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dungeon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARSMapGenerator> MapGeneratorClass; // 맵 생성기 클래스

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dungeon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ARSMapGenerator> MapGeneratorInstance; // 인스턴스
public:
	ARSMapGenerator* GetMapGenerator();
#pragma endregion

#pragma region SpawnManager
public:
	virtual URSSpawnManager* GetSpawnManager() const override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dungeon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<URSSpawnManager> SpawnManagerClass; // 던전 오브젝트 매니저 클래스

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dungeon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URSSpawnManager> SpawnManager;// 인스턴스
#pragma endregion

#pragma region Dungeon Info
public:
	int32 GetSeed() const;
	void InitRandSeed();

	int32 GetLevelIndex() const;
	void IncrementAtLevelIndex();

	int32 GetMaxStageCount() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dungeon Info", meta = (AllowPrivateAccess = "true"))
	int32 Seed;	// 해당 값을 기준으로 맵 생성

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dungeon Info", meta = (AllowPrivateAccess = "true"))
	int32 LevelIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dungeon Info", meta = (AllowPrivateAccess = "true"))
	int32 MaxStageCount;
#pragma endregion


#pragma region SaveData
public:
	UFUNCTION()
	void SaveDungeonInfo();

private:
	void LoadDungeonInfo();
#pragma endregion
};