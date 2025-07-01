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

            // 던전 세이브 파일 제거
            SaveGameSubsystem->DeleteDungeonSaveFile();
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

    // 현재 세이브 할 데이터가 없습니다.
    //UGameplayStatics::SaveGameToSlot(DungeonStageSaveGame, SaveGameSubsystem->DungeonInfoSaveSlotName, 0);
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
        // 현재 로드 할 데이터가 없습니다.
    }
}
