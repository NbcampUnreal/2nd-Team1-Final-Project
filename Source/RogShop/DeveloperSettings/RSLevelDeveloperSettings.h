// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RSLevelDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class ROGSHOP_API URSLevelDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UWorld> BaseAreaLevel;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UWorld> DungeonLevel;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UWorld> TycoonLevel;
};
