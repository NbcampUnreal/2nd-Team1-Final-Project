// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RSSpawnManager.h"
#include "RSSpawnManagerAccessor.generated.h"

class URSSpawnManager;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URSSpawnManagerAccessor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGSHOP_API IRSSpawnManagerAccessor
{
	GENERATED_BODY()

public:
	virtual URSSpawnManager* GetSpawnManager() const = 0;
};
