// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RSDunPlayerCharacter.h"
#include "Engine/World.h"




#pragma region 생성자
ARSDungeonGameModeBase::ARSDungeonGameModeBase()// 생성자: 기본 Pawn 클래스를 제거
{
    DefaultPawnClass = nullptr;// 기본 Pawn 클래스를 비활성화
}


#pragma region BeginPlay 처리
void ARSDungeonGameModeBase::BeginPlay()// 게임이 시작될 때 호출됨
{
    CurrentMapType = EMapType::Forest;// 현재 맵 타입을 Forest로 설정

    SpawnMap(CurrentMapType);

    if (MapGeneratorInstance)
    {
        MapGeneratorInstance->OnMapFullyLoaded.AddDynamic(this, &ARSDungeonGameModeBase::OnMapReady);// 맵 로딩 완료 시 콜백 등록
    }

    OnBossDead.AddDynamic(this, &ARSDungeonGameModeBase::SpawnDunNextStagePortal);
}


#pragma region 맵 스폰
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

void ARSDungeonGameModeBase::SpawnDunNextStagePortal()
{
    if (DunNextStagePortalClass)
    {
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.Owner = this;
        SpawnParameters.Instigator = nullptr;

        // TODO : 보스 맵의 특정 위치에 생성
        DunNextStagePortalInstance = GetWorld()->SpawnActor<AActor>(DunNextStagePortalClass, FTransform(), SpawnParameters);
    }
}

#pragma region 맵 로딩 완료 후 처리
void ARSDungeonGameModeBase::OnMapReady()// 맵 로딩이 완료되었을 때 호출되는 함수
{
    UE_LOG(LogTemp, Warning, TEXT("맵 로딩 완료, 캐릭터 생성 시작"));// 로그 출력


    TWeakObjectPtr<ARSDungeonGameModeBase> WeakThis(this);

    GetWorld()->GetTimerManager().SetTimer(WaitForMapHandle, [WeakThis]() {
        if (!WeakThis.IsValid()) return;// 함수 종료

        ARSDungeonGameModeBase* StrongThis = WeakThis.Get();

        if (!StrongThis->SpawnManager)
        {
            StrongThis->SpawnManager = NewObject<URSSpawnManager>(StrongThis, URSSpawnManager::StaticClass());// 스폰 매니저 객체 생성

            StrongThis->SpawnManager->Initialize(StrongThis->GetWorld(), StrongThis->GetGameInstance(), StrongThis->ShopNPCClass);// 스폰 매니저 초기화

            StrongThis->SpawnManager->SpawnPlayerAtStartPoint(StrongThis->PlayerClass);// 플레이어 생성
            StrongThis->SpawnManager->SpawnMonstersInLevel();// 몬스터 스폰
            StrongThis->SpawnManager->SpawnShopNPCInLevel();// 상점 NPC 스폰
        }
    }, 0.5f, false);
}


#pragma region 종료 처리
void ARSDungeonGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)// 게임 종료 시 호출됨
{

#pragma region 종료 처리
    Super::EndPlay(EndPlayReason);// 게임 종료 시 호출됨

    if (MapGeneratorInstance)
    {
        MapGeneratorInstance->OnMapFullyLoaded.RemoveDynamic(this, &ARSDungeonGameModeBase::OnMapReady);// 델리게이트에서 콜백 제거
    }
}

#pragma endregion
