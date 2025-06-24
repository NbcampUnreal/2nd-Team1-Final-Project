// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFlowInfoData.generated.h"

class UImage;

USTRUCT(BlueprintType)
struct ROGSHOP_API FGameFlowInfoData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText CategoryText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText PageText;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UTexture2D> GameInfoImage;
	
};
