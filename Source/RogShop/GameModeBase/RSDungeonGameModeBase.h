// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RSSpawnManager.h"
#include "RSMapGenerator.h"
#include "RSDungeonGameModeBase.generated.h"

// 던전 맵 타입을 나타내는 열거형
UENUM(BlueprintType)
enum class EMapType : uint8
{
	Forest     UMETA(DisplayName = "숲"),
	Desert     UMETA(DisplayName = "사막"),
	Cave       UMETA(DisplayName = "동굴")
};

// 던전 게임모드 클래스 정의
UCLASS()
class ROGSHOP_API ARSDungeonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
#pragma region 공개 함수
	ARSDungeonGameModeBase(); // 생성자
	virtual void BeginPlay() override; // 게임 시작 시 호출
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // 게임 종료 시 호출
	UFUNCTION()
	void OnMapReady(); // 맵이 완전히 로드되었을 때 실행되는 콜백
#pragma endregion

private:
#pragma region 비공개 함수
	void SpawnMap(EMapType MapType); // 선택된 맵 타입에 따라 맵 생성
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Type")
	EMapType CurrentMapType; // 현재 맵 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	TSubclassOf<AActor> ShopNPCClass; // 상점 NPC 클래스

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	TSubclassOf<AActor> PlatformClass; // 플랫폼 액터 클래스

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	TSubclassOf<ARSMapGenerator> ForestMapGeneratorClass; // 숲 맵 생성기 클래스

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	TSubclassOf<ARSMapGenerator> DesertMapGeneratorClass; // 사막 맵 생성기 클래스

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	TSubclassOf<ARSMapGenerator> CaveMapGeneratorClass; // 동굴 맵 생성기 클래스

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ACharacter> PlayerClass; // 플레이어 캐릭터 클래스
#pragma endregion

private:
	FTimerHandle WaitForMapHandle; // 맵 로딩 후 딜레이 핸들
};