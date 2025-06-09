// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DungeonObjectData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ROGSHOP_API FDungeonObjectData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ObjectClass;
};
