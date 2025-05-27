// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RSDataSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
public:
	UPROPERTY()
	TObjectPtr<UDataTable> Food;
	
	UPROPERTY()
	TObjectPtr<UDataTable> Ingredient;

	UPROPERTY()
	TObjectPtr<UDataTable> Weapon;

	UPROPERTY()
	TObjectPtr<UDataTable> Relic;
};
