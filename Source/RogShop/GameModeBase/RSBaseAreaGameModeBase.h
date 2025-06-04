// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RSBaseAreaGameModeBase.generated.h"

class ARSDungeonGroundWeapon;

UCLASS()
class ROGSHOP_API ARSBaseAreaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    const TArray<FName>& GetSpawnedWeaponRowNames() const;
    void AddSpawnedWeaponRowName(const FName& RowName);

private:
    UPROPERTY()
    TArray<FName> SpawnedWeaponRowNames;
};
