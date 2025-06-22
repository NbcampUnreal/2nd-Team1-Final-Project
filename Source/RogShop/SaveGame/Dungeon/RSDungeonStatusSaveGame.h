// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RSDungeonStatusSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSDungeonStatusSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP; // 플레이어 캐릭터의 최대 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HP; // 플레이어 캐릭터의 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed; // 플레이어 캐릭터의 이동 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPower;	// 플레이어 캐릭터의 공격력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed;	// 플레이어 캐릭터의 공격 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LifeEssence;	// 던전 재화
	
};
