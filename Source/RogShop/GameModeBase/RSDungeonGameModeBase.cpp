// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonGameModeBase.h"              
#include "RSSpawnManager.h"                         
#include "Kismet/GameplayStatics.h"                   
#include "GameFramework/Character.h"            
#include "DungeonLevelData.h"
#include "GameFramework/CharacterMovementComponent.h"  
#include "RSDunPlayerController.h"
#include "RSDataSubsystem.h"
#include "Engine/World.h"                             
#include "TimerManager.h"                             
#include "RogShop/UtilDefine.h"
#include "RSSaveGameSubsystem.h"
#include "RSDungeonStageSaveGame.h"

ARSDungeonGameModeBase::ARSDungeonGameModeBase()
{
}


void ARSDungeonGameModeBase::BeginPlay()// 게임이 시작될 때 호출됨
{
    Super::BeginPlay();
    UGameInstance* CurGameInstance = GetGameInstance();
    if (CurGameInstance)
    {
        URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
        if (SaveGameSubsystem)
        {
            SaveGameSubsystem->OnSaveRequested.AddDynamic(this, &ARSDungeonGameModeBase::SaveDungeonInfo);
        }
    }

    LoadDungeonInfo();

    ACharacter* PlayerChar = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    if (PlayerChar)
    {
        // 숨김 + 충돌 제거 + 중력 및 무브먼트 비활성화
        PlayerChar->SetActorHiddenInGame(true);
        PlayerChar->SetActorEnableCollision(false);

        if (UCharacterMovementComponent* MoveComp = PlayerChar->GetCharacterMovement())
        {
            MoveComp->DisableMovement();
            MoveComp->GravityScale = 0.0f;
        }
    }

    SpawnMap();
}


void ARSDungeonGameModeBase::SpawnMap()// 선택된 맵 타입에 따라 맵 생성기 액터를 스폰
{
    URSDataSubsystem* DataSubsystem = GetGameInstance()->GetSubsystem<URSDataSubsystem>();
    if (!DataSubsystem)
    {
        return;
    }

    TArray<FDungeonLevelData*> AllGroups;
    DataSubsystem->DungeonLevel->GetAllRows(TEXT("LevelRowData"), AllGroups);

    if (AllGroups.Num() == 0)
    {
        RS_LOG_DEBUG("LevelDataTable에 데이터가 없습니다.");
        return;
    }

    FDungeonLevelData* Level = AllGroups[TileIndex];

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //무슨일이 있어도 스폰// 충돌 무시하고 항상 스폰하도록 설정

    FVector Location = FVector::ZeroVector;
    FRotator Rotation = FRotator::ZeroRotator;

    MapGeneratorInstance = GetWorld()->SpawnActor<ARSMapGenerator>(MapGeneratorClass, Location, Rotation, SpawnParams);// 해당 맵 생성기 액터를 월드에 스폰
    MapGeneratorInstance->SetTileType(Level->GridSize, Level->TileSize,
        Level->LineTileLevel[FMath::RandRange(0, Level->LineTileLevel.Num() - 1)], Level->CornerTileLevel[FMath::RandRange(0, Level->CornerTileLevel.Num() - 1)], Level->CrossTileLevel[FMath::RandRange(0, Level->CrossTileLevel.Num() - 1)],
        Level->TTileLevel[FMath::RandRange(0, Level->TTileLevel.Num() - 1)], Level->DeadEndTileLevel[FMath::RandRange(0, Level->DeadEndTileLevel.Num() - 1)], Level->BossArenaLevel[FMath::RandRange(0, Level->BossArenaLevel.Num() - 1)], Level->EnvLevel[FMath::RandRange(0, Level->EnvLevel.Num() - 1)]);
    MapGeneratorInstance->StartMapGenerator();
}

int32 ARSDungeonGameModeBase::GetSeed() const
{
    return Seed;
}

void ARSDungeonGameModeBase::InitRandSeed()
{
    Seed = FMath::RandRange(1, INT32_MAX);
}

int32 ARSDungeonGameModeBase::GetTileIndex() const
{
    return TileIndex;
}

void ARSDungeonGameModeBase::IncrementAtTileIndex()
{
    TileIndex += 1;
}

void ARSDungeonGameModeBase::SaveDungeonInfo()
{
    // SaveGame 오브젝트 생성
    URSDungeonStageSaveGame* DungeonStageSaveGame = Cast<URSDungeonStageSaveGame>(UGameplayStatics::CreateSaveGameObject(URSDungeonStageSaveGame::StaticClass()));
    if (!DungeonStageSaveGame)
    {
        return;
    }

    // 세이브
    DungeonStageSaveGame->TileIndex = TileIndex;
    DungeonStageSaveGame->Seed = Seed;

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

void ARSDungeonGameModeBase::LoadDungeonInfo()
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
        TileIndex = DungeonInfoLoadGame->TileIndex;
        Seed = DungeonInfoLoadGame->Seed;
    }

    // 시드 값이 설정되어있지 않은 경우 랜덤한 시드를 설정한다.
    if (Seed == 0)
    {
        InitRandSeed();
    }
}

void ARSDungeonGameModeBase::OnMapReady()// 맵 로딩이 완료되었을 때 호출되는 함수
{
    RS_LOG_DEBUG("맵 로딩 완료, 캐릭터 생성 시작");
    
    TWeakObjectPtr<ARSDungeonGameModeBase> WeakThis(this);

    GetWorld()->GetTimerManager().SetTimerForNextTick([WeakThis]()
    {
        if (!WeakThis.IsValid()) return;


        ARSDungeonGameModeBase* GameMode = WeakThis.Get();

        if (!GameMode->SpawnManager)
        {
            RS_LOG_DEBUG("스폰 매니저 생성");
            GameMode->SpawnManager = NewObject<URSSpawnManager>(GameMode, URSSpawnManager::StaticClass());
            GameMode->SpawnManager->Initialize(GameMode->GetWorld(), GameMode->GetGameInstance(), GameMode->TileIndex);

            GameMode->SpawnManager->SpawnPlayerAtStartPoint();
            GameMode->SpawnManager->SpawnMonstersInLevel();
            GameMode->SpawnManager->SpawnShopNPCInLevel();
            GameMode->SpawnManager->SpawnBossPortal(GameMode->MapGeneratorInstance->BossWorldLocation);
            GameMode->SpawnManager->SpawnBossMonster();

            APlayerController* PC = UGameplayStatics::GetPlayerController(GameMode, 0);
            ARSDunPlayerController* DunPC = Cast<ARSDunPlayerController>(PC);

            if (DunPC)
            {
                DunPC->HideLoadingUI();
            }
        }
    });
}

void ARSDungeonGameModeBase::NotifyMapReady()
{
    RS_LOG_DEBUG("GameMode::NotifyMapReady - 델리게이트 Broadcast");
    OnMapFullyLoaded.Broadcast();
    OnMapReady();
}
