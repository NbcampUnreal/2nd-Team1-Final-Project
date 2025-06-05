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

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Melee,
	Range,
	Utillity,
};

USTRUCT(BlueprintType)
struct FMonsterIngredientsData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FName IngredientName;

	UPROPERTY(EditDefaultsOnly)
	float DropRate;  // 드랍 확률 (0~1)
};

USTRUCT(BlueprintType)
struct FMonsterAttackTraceData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FVector TraceBoxHalfSize;	// 트레이스 반경 (옆으로 얼마만큼 쏠지)

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

USTRUCT(BlueprintType)
struct FMonsterAttackSkillData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FName SkillName;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(EditDefaultsOnly)
	int32 PlayRate;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly)
	FMonsterAttackTraceData AttackTrace;

	UPROPERTY(EditDefaultsOnly)
	ESkillType skillType;
};

USTRUCT(BlueprintType)
struct ROGSHOP_API FMonsterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	EMonsterType MonsterType;

	UPROPERTY(EditDefaultsOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly)
	TArray<FMonsterAttackSkillData> MonsterAttackSkills;

	UPROPERTY(EditDefaultsOnly)
	TArray<FMonsterIngredientsData> Ingredients;

	UPROPERTY(EditDefaultsOnly)
	float MaxHP;

	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARSDunMonsterCharacter> MonsterClass;

};
