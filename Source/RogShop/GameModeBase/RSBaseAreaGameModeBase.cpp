// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseAreaGameModeBase.h"
#include "RogShop/UtilDefine.h"
#include "RSSpawnManager.h"

void ARSBaseAreaGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    if (SpawnManagerClass)
    {
        RS_LOG_DEBUG("스폰 매니저 생성");
        SpawnManager = NewObject<URSSpawnManager>(this, SpawnManagerClass);
    }
}

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

URSSpawnManager* ARSBaseAreaGameModeBase::GetSpawnManager() const
{
    return SpawnManager;
}
