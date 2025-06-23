// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseAreaGameModeBase.h"
#include "RogShop/UtilDefine.h"
#include "RSSpawnManager.h"
#include "RSSaveGameSubsystem.h"
#include "RSDungeonStageSaveGame.h"

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

    UGameInstance* CurGameInstance = GetGameInstance();
    if (CurGameInstance)
    {
        URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
        if (SaveGameSubsystem)
        {
            SaveGameSubsystem->OnSaveRequested.AddDynamic(this, &ARSBaseAreaGameModeBase::SaveDungeonInfo);
        }
    }

    LoadDungeonInfo();
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

void ARSBaseAreaGameModeBase::SaveDungeonInfo()
{
    // SaveGame 오브젝트 생성
    URSDungeonStageSaveGame* DungeonStageSaveGame = Cast<URSDungeonStageSaveGame>(UGameplayStatics::CreateSaveGameObject(URSDungeonStageSaveGame::StaticClass()));
    if (!DungeonStageSaveGame)
    {
        return;
    }

    // 세이브
    if (SpawnManager)
    {
        TSet<FName> UnspawnedWeapons = SpawnManager->GetUnspawnedWeapons();
        TSet<FName> UnspawnedRelics = SpawnManager->GetUnspawnedRelics();
        DungeonStageSaveGame->UnspawnedWeapons = UnspawnedWeapons.Array();
        DungeonStageSaveGame->UnspawnedRelics = UnspawnedWeapons.Array();
    }

    // 저장
    UGameInstance* CurGameInstance = GetGameInstance();
    if (!CurGameInstance)
    {
        return;
    }

    URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
    if (!SaveGameSubsystem)
    {
        return;
    }

    UGameplayStatics::SaveGameToSlot(DungeonStageSaveGame, SaveGameSubsystem->DungeonInfoSaveSlotName, 0);
}

void ARSBaseAreaGameModeBase::LoadDungeonInfo()
{
    // 저장된 세이브 로드
    UGameInstance* CurGameInstance = GetGameInstance();
    if (!CurGameInstance)
    {
        return;
    }

    URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
    if (!SaveGameSubsystem)
    {
        return;
    }

    URSDungeonStageSaveGame* DungeonInfoLoadGame = Cast<URSDungeonStageSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSubsystem->DungeonInfoSaveSlotName, 0));
    if (DungeonInfoLoadGame)
    {
        if (SpawnManager)
        {
            SpawnManager->SetUnspawnedWeapons(DungeonInfoLoadGame->UnspawnedWeapons);
            SpawnManager->SetUnspawnedRelics(DungeonInfoLoadGame->UnspawnedRelics);
        }
    }
    // 세이브 파일이 없는 경우
    else
    {
        SpawnManager->ResetUnspawnedWeapons();
        SpawnManager->ResetUnspawnedRelics();
    }
}
