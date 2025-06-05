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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SkillName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 PlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMonsterAttackTraceData AttackTrace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESkillType skillType;
};

USTRUCT(BlueprintType)
struct ROGSHOP_API FMonsterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EMonsterType MonsterType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMonsterAttackSkillData> MonsterAttackSkills;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FMonsterIngredientsData> Ingredients;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ARSDunMonsterCharacter> MonsterClass;

};
