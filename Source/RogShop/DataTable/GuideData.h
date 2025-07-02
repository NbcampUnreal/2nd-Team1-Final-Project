// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GuideData.generated.h"

USTRUCT(BlueprintType)
struct ROGSHOP_API FGuideData : public FTableRowBase
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ButtonTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MultiLine=true))
    FText GuideDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTexture2D> GuideImage;
};