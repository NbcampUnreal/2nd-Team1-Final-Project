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
	float MaxHP;

	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARSDunMonsterCharacter> MonsterClass;

};
