// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RSTycoonSaveGame.generated.h"

struct FItemSlot;

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemSlot> Ingredients;
};
