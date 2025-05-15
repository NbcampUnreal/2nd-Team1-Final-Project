// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EBaseDunAIState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EBaseDunAIState : uint8
{
    Walking = 0             UMETA(DisplayName = "Walking"),
    Chasing = 1             UMETA(DisplayName = "Chasing")
};
