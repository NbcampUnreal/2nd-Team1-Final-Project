// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterSpawnGroupData.generated.h"

USTRUCT(BlueprintType)
struct FMonsterCount
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName MonsterRowName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Count;
};

USTRUCT(BlueprintType)
struct ROGSHOP_API FMonsterSpawnGroupData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMonsterCount> SpawnGroup;
};
