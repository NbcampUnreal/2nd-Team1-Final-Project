// Fill out your copyright notice in the Description page of Project Settings.


#include "RSSpawnManager.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "MonsterSpawnGroupData.h"
#include "MonsterData.h"
#include "Algo/RandomShuffle.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RSDunMonsterCharacter.h"
#include "RSDataSubsystem.h"
#include "RSDunPlayerCharacter.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"

// 외부에서 전달받은 월드와 몬스터 테이블을 초기화
void URSSpawnManager::Initialize(UWorld* InWorld, UGameInstance* GameInstance, TSubclassOf<AActor> ShopNPC)
{
    World = InWorld;
    ShopNPCClass = ShopNPC;
    UGameInstance* CurGameInstance = GameInstance;
    if (!CurGameInstance)
    {
        return;
    }
    URSDataSubsystem* DataSubsystem = CurGameInstance->GetSubsystem<URSDataSubsystem>();
    if (!DataSubsystem)
    {
        return;
    }
    MonsterRawTable = DataSubsystem->ForestMonsterSpawnGroup;
    MonsterStateTable = DataSubsystem->MonsterStateGroup;
    if (!MonsterRawTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed Update MonsterRawTable"));
        return;
    }
    if (!MonsterStateTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed Update MonsterStateGroup"));
        return;
    }
}

// 로드된 레벨에 있는 모든 TargetPoint를 찾아 몬스터 스폰
void URSSpawnManager::SpawnMonstersInLevel()
{
    if (!World || !MonsterRawTable || !MonsterStateTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("몬스터 스폰 실패"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("몬스터 스폰 시작"));
    TArray<AActor*> MonsterTargets;
    for (TActorIterator<ATargetPoint> It(World); It; ++It)
    {
        if (It->Tags.Contains(FName("Monster")))
        {
            MonsterTargets.Add(*It);
        }
    }

    Algo::RandomShuffle(MonsterTargets);

    int32 TargetIndex = 0;

    TArray<FMonsterSpawnGroupData*> AllGroups;
    MonsterRawTable->GetAllRows(TEXT("MonsterRawData"), AllGroups);

    for (FMonsterSpawnGroupData* Group : AllGroups)
    {
        for (const FMonsterCount& Entry : Group->SpawnGroup)
        {
            FName RowName = Entry.MonsterRowName;
            int32 Count = Entry.Count;

            FMonsterData* StateRow = MonsterStateTable->FindRow<FMonsterData>(RowName, TEXT("MonsterStateLookup"));
            if (!StateRow || !StateRow->MonsterClass)
            {
                continue;
            }

            for (int32 i = 0; i < Count && TargetIndex < MonsterTargets.Num(); ++i)
            {
                AActor* Target = MonsterTargets[TargetIndex++];

                FTransform SpawnTransform = Target->GetActorTransform();

                FActorSpawnParameters Params;
                Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                ARSDunMonsterCharacter* Monster =  World->SpawnActor<ARSDunMonsterCharacter>(StateRow->MonsterClass, SpawnTransform, Params);

                FVector SpawnLocation = SpawnTransform.GetLocation();
                SpawnLocation.Z += Monster->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
                Monster->SetActorLocation(SpawnLocation);

                Monster->IncreaseHP(StateRow->MaxHP);
                Monster->ChangeMoveSpeed(StateRow->MoveSpeed);
            }
        }
    }
}

void URSSpawnManager::SpawnShopNPCInLevel()
{
    if (!World || !ShopNPCClass) 
    {
        UE_LOG(LogTemp, Warning, TEXT("상점 생성 실패"));
        return;
    } 

    TArray<ATargetPoint*> ShopPoints;

    // 모든 TargetPoint 중 이름이 일치하는 것들을 후보로 수집
    for (TActorIterator<ATargetPoint> It(World); It; ++It)
    {
        ATargetPoint* Point = *It;
        if (Point && Point->Tags.Contains(FName("NPC")))
        {
            ShopPoints.Add(Point);
        }
    }

    // 무작위로 하나 선택
    if (ShopPoints.Num() == 0) 
    {
        return;
    } 
    
    int32 Index = FMath::RandRange(0, ShopPoints.Num() - 1);
    ATargetPoint* ChosenPoint = ShopPoints[Index];

    FTransform SpawnTransform = ChosenPoint->GetActorTransform();

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    World->SpawnActor<AActor>(ShopNPCClass, SpawnTransform,SpawnParams);
}

void URSSpawnManager::SpawnPlayerAtStartPoint(TSubclassOf<ACharacter> PlayerClass)
{
    if (!World || !PlayerClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed CratePlayerCharacter"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("In PlayerCharacter"));

    ATargetPoint* StartPoint = nullptr;

    for (TActorIterator<ATargetPoint> It(World); It; ++It)
    {
        ATargetPoint* Point = *It;
        if (Point && Point->Tags.Contains(FName("Player")))
        {
            UE_LOG(LogTemp, Warning, TEXT("찾은 위치: %s"), *Point->GetActorLocation().ToString());

            if(FVector::Dist(Point->GetActorLocation(), FVector::ZeroVector) < 3000.f)
            {
                UE_LOG(LogTemp, Warning, TEXT("Find StartPos"));
                StartPoint = Point;
                break;
            }
        }
    }

    if (!StartPoint) return;


    FTransform SpawnTransform = StartPoint->GetActorTransform();
    ACharacter* ExistingPlayer = UGameplayStatics::GetPlayerCharacter(World, 0);

    if (ExistingPlayer)
    {
        // 위치만 이동
        UE_LOG(LogTemp, Warning, TEXT("MovePlayerCharacter"));
        ExistingPlayer->SetActorTransform(SpawnTransform);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CratePlayerCharacter"));
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        // 새로 생성
        ACharacter* PlayerCharacter = World->SpawnActor<ACharacter>(PlayerClass, SpawnTransform, SpawnParams);

        FVector SpawnLocation = SpawnTransform.GetLocation();
        SpawnLocation.Z += PlayerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
        PlayerCharacter->SetActorLocation(SpawnLocation);

        if (PlayerCharacter)
        {
            APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
            if (PC)
            {
                PC->Possess(PlayerCharacter);
            }
        }
    }
}
