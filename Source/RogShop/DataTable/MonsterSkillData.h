#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // 필수 헤더
#include "MonsterSkillData.generated.h"

USTRUCT(BlueprintType)
struct ROGSHOP_API FMonsterSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FName SkillName;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(EditDefaultsOnly)
	float AttackPower;

	UPROPERTY(EditDefaultsOnly)
	float PlayRate = 1.0f;		// 각 애니메이션 재생속도를 조절해서 공격 속도를 조절

	/*UPROPERTY(EditAnywhere)
	float Cooldown = 2.0f;*/    // TODO :: 나중에 전투 시스템 확장되면 쿨타임 추가, 아니면 제거

	UPROPERTY(EditDefaultsOnly)
	bool bIsBasicAttack;		// 기본 공격과 다른 스킬을 구분하기 위한 변수

	// 트레이스 관련
	UPROPERTY(EditDefaultsOnly)
	FVector TraceBoxHalfSize;   // 트레이스 반경 (옆으로 얼만큼 쏠지)

	UPROPERTY(EditDefaultsOnly)
	float TraceLength;			// 트레이스 길이 (앞으로 얼마만큼 쏠지)

	UPROPERTY(EditDefaultsOnly)
	float TraceForwardOffset;	// 소켓 시작점 앞뒤 보정

	UPROPERTY(EditDefaultsOnly)
	float TraceRightOffset;     // 소켓 시작점 좌우 보정

	UPROPERTY(EditDefaultsOnly)
	float TraceUpOffset;        // 소켓 시작점 높이 보정

	UPROPERTY(EditDefaultsOnly)
	FName SocketLocation;		// 트레이스가 쏴질 소켓의 시작점
	
};
