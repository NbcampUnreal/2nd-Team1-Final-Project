#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterData.generated.h"

class ARSDunMonsterCharacter;

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	Common,
	Elite,
	Boss,
};

//USTRUCT(BlueprintType)
//struct FMonsterAttackTraceData
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	FVector TraceBoxHalfSize;	// 트레이스 반경 (옆으로 얼마만큼 쏠지)
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	float TraceLength;			// 트레이스 길이 (앞으로 얼마만큼 쏠지)
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	float TraceForwardOffset;	// 소켓 시작점 앞뒤 보정
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	float TraceRightOffset;     // 소켓 시작점 좌우 보정
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	float TraceUpOffset;        // 소켓 시작점 높이 보정
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	FName SocketLocation;		// 트레이스가 쏴질 소켓의 시작점
//
//};
//
//USTRUCT(BlueprintType)
//struct FMonsterAttackSkillData
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	FName SkillName;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	TObjectPtr<UAnimMontage> SkillMontage;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	int32 PlayRate;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	float Damage;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	FMonsterAttackTraceData AttackTrace;
//};

USTRUCT(BlueprintType)
struct ROGSHOP_API FMonsterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	EMonsterType MonsterType;

	UPROPERTY(EditDefaultsOnly)
	FText Description;

	/*UPROPERTY(EditDefaultsOnly)
	TArray<FMonsterAttackSkillData> MonsterAttackSkills;*/

	UPROPERTY(EditDefaultsOnly)
	float MaxHP;

	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARSDunMonsterCharacter> MonsterClass;

};
