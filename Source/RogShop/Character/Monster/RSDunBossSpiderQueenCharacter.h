// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunMonsterCharacter.h"
#include "RSDunBossSpiderQueenCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSDunBossSpiderQueenCharacter : public ARSDunMonsterCharacter
{
	GENERATED_BODY()
	
public:
	ARSDunBossSpiderQueenCharacter();

	void PlayBaseAttackAnim() override;
	void PlayHitReactAnim() override;
	void PlayDeathAnim() override;

	void PerformAttackTrace() override;

//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackTrace")
//	FVector TraceBoxHalfSize;
//
//	// 트레이스 길이 (앞으로 얼마만큼 쏠지)
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackTrace")
//	float TraceLength;
//
//	UPROPERTY(EditAnywhere, Category = "AttackTrace")
//	float TraceForwardOffset; // 소켓 기준 앞으로 얼마나 밀지
//
//	UPROPERTY(EditAnywhere, Category = "AttackTrace")
//	float TraceRightOffset;     // (선택) 좌우 보정
//
//	UPROPERTY(EditAnywhere, Category = "AttackTrace")
//	float TraceUpOffset;        // (선택) 높이 보정

};
