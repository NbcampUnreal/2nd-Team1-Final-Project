// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void TravelToLevel(const TSoftObjectPtr<UWorld>& TargetLevelAsset) const;

public:
	TArray<FName> PurchasedItemIDs;
};
