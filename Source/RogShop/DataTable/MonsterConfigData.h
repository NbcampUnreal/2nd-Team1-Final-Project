#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // 필수 헤더
#include "MonsterConfigData.generated.h"

USTRUCT(BlueprintType)
struct ROGSHOP_API FMonsterConfigData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> DeathMontage;

};
