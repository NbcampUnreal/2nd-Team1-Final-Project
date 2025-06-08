// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonGameModeBase.h"              
#include "RSSpawnManager.h"                         
#include "Kismet/GameplayStatics.h"                   
#include "GameFramework/Character.h"                   
#include "GameFramework/CharacterMovementComponent.h"  
#include "Engine/World.h"                             
#include "TimerManager.h"                             
#include "RogShop/UtilDefine.h"

ARSDungeonGameModeBase::ARSDungeonGameModeBase()
{
}


void ARSDungeonGameModeBase::BeginPlay()// 게임이 시작될 때 호출됨
{
    Super::BeginPlay();

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

    CurrentMapType = EMapType::Forest;// 현재 맵 타입을 Forest로 설정

    SpawnMap(CurrentMapType);
}


void ARSDungeonGameModeBase::SpawnMap(EMapType MapType)// 선택된 맵 타입에 따라 맵 생성기 액터를 스폰
{
    switch (MapType)
    {
    case EMapType::Forest:
        if (ForestMapGeneratorClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //무슨일이 있어도 스폰// 충돌 무시하고 항상 스폰하도록 설정

            FVector Location = FVector::ZeroVector;
            FRotator Rotation = FRotator::ZeroRotator;

            MapGeneratorInstance = GetWorld()->SpawnActor<ARSMapGenerator>(ForestMapGeneratorClass, Location, Rotation, SpawnParams);// 해당 맵 생성기 액터를 월드에 스폰
        }
        break;
    case EMapType::Desert:
        if (DesertMapGeneratorClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //무슨일이 있어도 스폰// 충돌 무시하고 항상 스폰하도록 설정

            FVector Location = FVector::ZeroVector;
            FRotator Rotation = FRotator::ZeroRotator;

            MapGeneratorInstance = GetWorld()->SpawnActor<ARSMapGenerator>(DesertMapGeneratorClass, Location, Rotation, SpawnParams);// 해당 맵 생성기 액터를 월드에 스폰
        }
        break;
    case EMapType::Cave:
        if (CaveMapGeneratorClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //무슨일이 있어도 스폰// 충돌 무시하고 항상 스폰하도록 설정

            FVector Location = FVector::ZeroVector;
            FRotator Rotation = FRotator::ZeroRotator;

            MapGeneratorInstance = GetWorld()->SpawnActor<ARSMapGenerator>(CaveMapGeneratorClass, Location, Rotation, SpawnParams);// 해당 맵 생성기 액터를 월드에 스폰
        }
        break;
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
            GameMode->SpawnManager->Initialize(GameMode->GetWorld(), GameMode->GetGameInstance(), GameMode->ShopNPCClass,GameMode->DunNextStagePortalClass);

            GameMode->SpawnManager->SpawnPlayerAtStartPoint();
            GameMode->SpawnManager->SpawnMonstersInLevel();
            GameMode->SpawnManager->SpawnShopNPCInLevel();
            GameMode->SpawnManager->SpawnBossPortal(GameMode->MapGeneratorInstance->BossWorldLocation, GameMode->BossPortal);
            GameMode->SpawnManager->SpawnBossMonster();
        }
    });
}

void ARSDungeonGameModeBase::NotifyMapReady()
{
    RS_LOG_DEBUG("GameMode::NotifyMapReady - 델리게이트 Broadcast");
    OnMapFullyLoaded.Broadcast();
    OnMapReady();
}
