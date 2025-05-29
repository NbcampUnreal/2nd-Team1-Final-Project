// Fill out your copyright notice in the Description page of Project Settings.


#include "RSSpawnManager.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// �ܺο��� ���޹��� ����� ���� ���̺��� �ʱ�ȭ
void URSSpawnManager::Initialize(UWorld* InWorld, UDataTable* InDataTable, TSubclassOf<AActor> ShopNPC)
{
    World = InWorld;
    MonsterTable = InDataTable;
    ShopNPCClass = ShopNPC;
}

// �ε�� ������ �ִ� ��� TargetPoint�� ã�� ���� ����
void URSSpawnManager::SpawnMonstersInLevel(ULevel* LoadedLevel)
{
    if (!World || !MonsterTable || !LoadedLevel) return;

    for (AActor* Actor : LoadedLevel->Actors)
    {
        // TargetPoint���� Ȯ�� �� ����
        if (Actor && Actor->IsA<ATargetPoint>() && Actor->Tags.Contains(FName("Monster")))
        {
            SpawnMonsterAtTarget(Actor);
        }
    }
}

// ������ ��ġ(TargetPoint)���� ���� �ϳ� ����
AActor* URSSpawnManager::SpawnMonsterAtTarget(AActor* TargetPoint)
{
    // DataTable���� ���� Ŭ���� ����
    TSubclassOf<AActor> MonsterClass = SelectMonsterClass();
    if (!MonsterClass) return nullptr;

    // TargetPoint ��ġ���� ����
    FTransform SpawnTransform = TargetPoint->GetActorTransform();
    return World->SpawnActor<AActor>(MonsterClass, SpawnTransform);
}

TSubclassOf<AActor> URSSpawnManager::SelectMonsterClass()
{
    if (!MonsterTable) return nullptr;

    TArray<FMonsterSpawnData*> AllRows;
    MonsterTable->GetAllRows(TEXT("Monster Data"), AllRows);

    if (AllRows.Num() == 0) return nullptr;

    // ������ �ε����� ���� Ŭ���� ����
    int32 Index = FMath::RandRange(0, AllRows.Num() - 1);
    return AllRows[Index]->MonsterClass;
}

void URSSpawnManager::SpawnShopNPCInLevel(ULevel* LoadedLevel)
{
    if (!World || !ShopNPCClass || !LoadedLevel) return;

    TArray<ATargetPoint*> ShopPoints;

    // ��� TargetPoint �� �̸��� ��ġ�ϴ� �͵��� �ĺ��� ����
    for (AActor* Actor : LoadedLevel->Actors)
    {
        if (Actor && Actor->IsA<ATargetPoint>() && Actor->Tags.Contains("NPC"))
        {
            ShopPoints.Add(Cast<ATargetPoint>(Actor));
        }
    }

    // �������� �ϳ� ����
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
        //TargetPoint���� Ȯ��
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
        // ��ġ�� �̵�
        ExistingPlayer->SetActorTransform(SpawnTransform);
    }
    else
    {
        // ���� ����
        World->SpawnActor<ACharacter>(PlayerClass, SpawnTransform);
    }
}
