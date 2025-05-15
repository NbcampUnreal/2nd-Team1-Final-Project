// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "RSCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	UFUNCTION(exec)
	void TestMonsterWalk();

	UFUNCTION(exec)
	void OpenTycoonLevel();
};
