// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EBaseTycoonAIState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EBaseTycoonAIState : uint8
{
    Walking = 0       UMETA(DisplayName = "Walking"),
    Working = 1     UMETA(DisplayName = "Working"),
    Waiting = 2   UMETA(DisplayName = "Waiting")
};
