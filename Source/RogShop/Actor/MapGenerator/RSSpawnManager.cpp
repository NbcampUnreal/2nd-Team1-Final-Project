// Fill out your copyright notice in the Description page of Project Settings.


#include "RSSpawnManager.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// 외부에서 전달받은 월드와 몬스터 테이블을 초기화
void URSSpawnManager::Initialize(UWorld* InWorld, UDataTable* InDataTable, TSubclassOf<AActor> ShopNPC)
{
    World = InWorld;
    MonsterTable = InDataTable;
    ShopNPCClass = ShopNPC;
}

// 로드된 레벨에 있는 모든 TargetPoint를 찾아 몬스터 스폰
void URSSpawnManager::SpawnMonstersInLevel(ULevel* LoadedLevel)
{
    if (!World || !MonsterTable || !LoadedLevel) return;

    for (AActor* Actor : LoadedLevel->Actors)
    {
        // TargetPoint인지 확인 후 스폰
        if (Actor && Actor->IsA<ATargetPoint>() && Actor->Tags.Contains(FName("Monster")))
        {
            SpawnMonsterAtTarget(Actor);
        }
    }
}

// 지정된 위치(TargetPoint)에서 몬스터 하나 스폰
AActor* URSSpawnManager::SpawnMonsterAtTarget(AActor* TargetPoint)
{
    // DataTable에서 몬스터 클래스 선택
    TSubclassOf<AActor> MonsterClass = SelectMonsterClass();
    if (!MonsterClass) return nullptr;

    // TargetPoint 위치에서 스폰
    FTransform SpawnTransform = TargetPoint->GetActorTransform();
    return World->SpawnActor<AActor>(MonsterClass, SpawnTransform);
}

TSubclassOf<AActor> URSSpawnManager::SelectMonsterClass()
{
    if (!MonsterTable) return nullptr;

    TArray<FMonsterSpawnData*> AllRows;
    MonsterTable->GetAllRows(TEXT("Monster Data"), AllRows);

    if (AllRows.Num() == 0) return nullptr;

    // 무작위 인덱스로 몬스터 클래스 선택
    int32 Index = FMath::RandRange(0, AllRows.Num() - 1);
    return AllRows[Index]->MonsterClass;
}

void URSSpawnManager::SpawnShopNPCInLevel(ULevel* LoadedLevel)
{
    if (!World || !ShopNPCClass || !LoadedLevel) return;

    TArray<ATargetPoint*> ShopPoints;

    // 모든 TargetPoint 중 이름이 일치하는 것들을 후보로 수집
    for (AActor* Actor : LoadedLevel->Actors)
    {
        if (Actor && Actor->IsA<ATargetPoint>() && Actor->Tags.Contains("NPC"))
        {
            ShopPoints.Add(Cast<ATargetPoint>(Actor));
        }
    }

    // 무작위로 하나 선택
    if (ShopPoints.Num() == 0) return;
    int32 Index = FMath::RandRange(0, ShopPoints.Num() - 1);
    ATargetPoint* ChosenPoint = ShopPoints[Index];

    FTransform SpawnTransform = ChosenPoint->GetActorTransform();
    World->SpawnActor<AActor>(ShopNPCClass, SpawnTransform);
}

void URSSpawnManager::SpawnPlayerAtStartPoint(ULevel* LoadedLevel, TSubclassOf<ACharacter> PlayerClass)
{
    if (!World || !LoadedLevel || !PlayerClass) return;

    ATargetPoint* StartPoint = nullptr;

    for (AActor* Actor : LoadedLevel->Actors)
    {
        //TargetPoint인지 확인
        if (!Actor || !Actor->IsA<ATargetPoint>()) continue;
        if (Actor->Tags.Contains(FName("Player")))
        {
            StartPoint = Cast<ATargetPoint>(Actor);
            break;
        }
    }
    FTransform SpawnTransform = StartPoint->GetActorTransform();
    ACharacter* ExistingPlayer = UGameplayStatics::GetPlayerCharacter(World, 0);

    if (ExistingPlayer)
    {
        // 위치만 이동
        ExistingPlayer->SetActorTransform(SpawnTransform);
    }
    else
    {
        // 새로 생성
        World->SpawnActor<ACharacter>(PlayerClass, SpawnTransform);
    }
}
