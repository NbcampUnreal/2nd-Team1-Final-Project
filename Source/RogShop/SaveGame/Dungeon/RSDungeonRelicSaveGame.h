// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RSDungeonRelicSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSDungeonRelicSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> RelicList;
};
