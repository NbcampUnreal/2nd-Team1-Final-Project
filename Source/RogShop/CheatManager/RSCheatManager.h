// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "RSCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	URSCheatManager();
	
public:
	// 던전 몬스터 애니메이션 테스트용 함수
	UFUNCTION(exec)
	void TestDunMonsterAttack();

	UFUNCTION(exec)
	void TestDunMonsterDeath();
  
	UFUNCTION(exec)
	void OpenTycoonLevel();
  
	// 어떤 던전 몬스터든지 스폰할 수 있는 함수
	UFUNCTION(Exec)
	void SpawnMonster(FString MonsterName);

	UFUNCTION(Exec)
	void SpawnDunShopNPC();
  
	UFUNCTION(exec)
	void SpawnWeaponPad();

	UFUNCTION(exec)
	void ShowRSDunMainWidget();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnMonster")
	TMap<FString, TSubclassOf<AActor>> MonsterMap;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AActor> WeaponSpawnPadBPClass;
};
