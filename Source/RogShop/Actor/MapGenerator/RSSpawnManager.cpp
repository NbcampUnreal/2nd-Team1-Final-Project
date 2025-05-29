// Fill out your copyright notice in the Description page of Project Settings.


#include "RSSpawnManager.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RSDataSubsystem.h"
#include "RSDunPlayerCharacter.h"
#include "Engine/World.h"

// 외부에서 전달받은 월드와 몬스터 테이블을 초기화
void URSSpawnManager::Initialize(UWorld* InWorld, UGameInstance* GameInstance,TSubclassOf<AActor> ShopNPC)
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
    MonsterTable = DataSubsystem->ForestMonsterSpawnGroup;
    if (!MonsterTable)
    {
        return;
    }
}

// 로드된 레벨에 있는 모든 TargetPoint를 찾아 몬스터 스폰
void URSSpawnManager::SpawnMonstersInLevel()
{
    if (!World || !MonsterTable) return;

    for (TActorIterator<ATargetPoint> It(World); It; ++It)
    {
        ATargetPoint* Point = *It;
        if (Point && Point->Tags.Contains(FName("Monster")))
        {
            SpawnMonsterAtTarget(Point);
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
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return World->SpawnActor<AActor>(MonsterClass, SpawnTransform,SpawnParams);
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

void URSSpawnManager::SpawnShopNPCInLevel()
{
    if (!World || !ShopNPCClass) 
    {
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
