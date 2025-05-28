// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Engine/TargetPoint.h"
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

	// ������ ���� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> MonsterClass;
};

UCLASS()
class ROGSHOP_API URSSpawnManager : public UObject
{
	GENERATED_BODY()

public:
	// �ܺο��� ����� DataTable�� �ʱ�ȭ�� �Լ�
	void Initialize(UWorld* InWorld, UDataTable* InDataTable, TSubclassOf<AActor> ShopNPC);
	// �־��� Level�� �ִ� TargetPoint���� ã�� ���� ����
	void SpawnMonstersInLevel(ULevel* LoadedLevel);
	// ����NPC������ ���� �Լ�
	void SpawnShopNPCInLevel(ULevel* LoadedLevel, const FString& TargetPointName = TEXT("BP_ShopSpawnPoint"));
	// Player������ ���� �Լ�
	void SpawnPlayerAtStartPoint(ULevel* LoadedLevel, TSubclassOf<ACharacter> PlayerClass);

private:
	// Ư�� TargetPoint ��ġ�� ���� �ϳ� ����
	AActor* SpawnMonsterAtTarget(AActor* TargetPoint);
	// DataTable���� �������� ���� Ŭ���� ����
	TSubclassOf<AActor> SelectMonsterClass();
	// ������ ���� ��ü ����

private:
	UWorld* World;
	// ���� ������ ��� �����Ϳ� ������ ���̺�
	UDataTable* MonsterTable;
	// ���� NPC Ŭ����
	TSubclassOf<AActor> ShopNPCClass;
};
