// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RSDunPlayerCharacter.h"
#include "Engine/World.h"



ARSDungeonGameModeBase::ARSDungeonGameModeBase()
{
    DefaultPawnClass = nullptr;
}

void ARSDungeonGameModeBase::BeginPlay()
{
    CurrentMapType = EMapType::Forest;

    SpawnMap(CurrentMapType);

 

    if (MapGeneratorInstance)
    {
        MapGeneratorInstance->OnMapFullyLoaded.AddDynamic(this, &ARSDungeonGameModeBase::OnMapReady);
    }
}

void ARSDungeonGameModeBase::SpawnMap(EMapType MapType)
{
    switch (MapType)
    {
    case EMapType::Forest:
        if (ForestMapGeneratorClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //무슨일이 있어도 스폰

            FVector Location = FVector::ZeroVector;
            FRotator Rotation = FRotator::ZeroRotator;

            MapGeneratorInstance = GetWorld()->SpawnActor<ARSMapGenerator>(ForestMapGeneratorClass, Location, Rotation, SpawnParams);
        }
        break;
    case EMapType::Desert:
        if (DesertMapGeneratorClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //무슨일이 있어도 스폰

            FVector Location = FVector::ZeroVector;
            FRotator Rotation = FRotator::ZeroRotator;

            MapGeneratorInstance = GetWorld()->SpawnActor<ARSMapGenerator>(DesertMapGeneratorClass, Location, Rotation, SpawnParams);
        }
        break;
    case EMapType::Cave:
        if (CaveMapGeneratorClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //무슨일이 있어도 스폰

            FVector Location = FVector::ZeroVector;
            FRotator Rotation = FRotator::ZeroRotator;

            MapGeneratorInstance = GetWorld()->SpawnActor<ARSMapGenerator>(CaveMapGeneratorClass, Location, Rotation, SpawnParams);
        }
        break;
    }
}
void ARSDungeonGameModeBase::OnMapReady()
{
    UE_LOG(LogTemp, Warning, TEXT("맵 로딩 완료, 캐릭터 생성 시작"));


    TWeakObjectPtr<ARSDungeonGameModeBase> WeakThis(this);

    GetWorld()->GetTimerManager().SetTimer(WaitForMapHandle, [WeakThis]() {
        if (!WeakThis.IsValid()) return;

        ARSDungeonGameModeBase* StrongThis = WeakThis.Get();

        if (!StrongThis->SpawnManager)
        {
            StrongThis->SpawnManager = NewObject<URSSpawnManager>(StrongThis, URSSpawnManager::StaticClass());

            StrongThis->SpawnManager->Initialize(StrongThis->GetWorld(), StrongThis->GetGameInstance(), StrongThis->ShopNPCClass);

            StrongThis->SpawnManager->SpawnPlayerAtStartPoint(StrongThis->PlayerClass);
            StrongThis->SpawnManager->SpawnMonstersInLevel();
            StrongThis->SpawnManager->SpawnShopNPCInLevel();
        }
    }, 0.5f, false);
}

void ARSDungeonGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (MapGeneratorInstance)
    {
        MapGeneratorInstance->OnMapFullyLoaded.RemoveDynamic(this, &ARSDungeonGameModeBase::OnMapReady);
    }
}