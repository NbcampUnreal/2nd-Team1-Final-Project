#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // 필수 헤더
#include "MonsterStatData.generated.h"

USTRUCT(BlueprintType)
struct ROGSHOP_API FMonsterStatData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float MaxHP;

	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly)
	float AttackPower;

	UPROPERTY(EditDefaultsOnly)
	float AttackSpeed;
	
};
