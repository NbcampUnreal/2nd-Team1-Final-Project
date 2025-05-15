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
	// ���� ���� �ִϸ��̼� �׽�Ʈ�� �Լ�
	UFUNCTION(exec)
	void TestDunMonsterAttack();

	UFUNCTION(exec)
	void TestDunMonsterHItReact();

	UFUNCTION(exec)
	void TestDunMonsterDeath();

	// � ���� ���͵��� ������ �� �ִ� �Լ�
	UFUNCTION(exec)
	void SpawnMonster(FString MonsterName);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnMonster")
	TMap<FString, TSubclassOf<AActor>> MonsterMap;

	UFUNCTION(Exec)
	void SpawnDunShopNPC();
};
