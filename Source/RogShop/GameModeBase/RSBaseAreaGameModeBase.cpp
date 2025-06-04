// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseAreaGameModeBase.h"
#include "RSDungeonGroundWeapon.h"
#include "RSDunPlayerController.h"

void ARSBaseAreaGameModeBase::BeginPlay()
{
    Super::BeginPlay();

}

const TArray<FName>& ARSBaseAreaGameModeBase::GetSpawnedWeaponRowNames() const
{
    return SpawnedWeaponRowNames;
}

void ARSBaseAreaGameModeBase::AddSpawnedWeaponRowName(const FName& RowName)
{
    SpawnedWeaponRowNames.Add(RowName);
}
