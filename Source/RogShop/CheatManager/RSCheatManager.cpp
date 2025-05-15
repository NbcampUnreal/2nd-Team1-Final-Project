// Fill out your copyright notice in the Description page of Project Settings.


#include "RSCheatManager.h"
#include "RSDunMonsterCharacter.h"
#include "RSDunAnubisCharacter.h"
#include "RSDunBossSpiderQueenCharacter.h"

#include "Kismet/GameplayStatics.h"

URSCheatManager::URSCheatManager()
{
    MonsterMap.Add("anubis", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunAnubisCharacter.BP_RSDunAnubisCharacter_C")));
    MonsterMap.Add("bossspiderqueen", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunBossSpiderQueenCharacter.BP_RSDunBossSpiderQueenCharacter_C")));

}

void URSCheatManager::TestDunMonsterAttack()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    ARSDunMonsterCharacter* Monster = Cast<ARSDunMonsterCharacter>(
        UGameplayStatics::GetActorOfClass(World, ARSDunMonsterCharacter::StaticClass())
    );

    if (Monster)
    {
        Monster->PlayBaseAttackAnim();
    }
}

void URSCheatManager::TestDunMonsterHItReact()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    ARSDunMonsterCharacter* Monster = Cast<ARSDunMonsterCharacter>(
        UGameplayStatics::GetActorOfClass(World, ARSDunMonsterCharacter::StaticClass())
    );

    if (Monster)
    {
        Monster->PlayHitReactAnim();
    }
}

void URSCheatManager::TestDunMonsterDeath()
{
    UE_LOG(LogTemp, Warning, TEXT("1"));
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("2"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("3"));
    ARSDunMonsterCharacter* Monster = Cast<ARSDunMonsterCharacter>(
        UGameplayStatics::GetActorOfClass(World, ARSDunMonsterCharacter::StaticClass())
    );

    if (Monster)
    {
        UE_LOG(LogTemp, Warning, TEXT("4"));
        Monster->PlayDeathAnim();
    }
}

void URSCheatManager::SpawnMonster(FString MonsterName)
{
    UWorld* World = GetWorld();
    if (!World) return;

    /*FVector SpawnLocation = GetOuterAPlayerController()->GetPawn()->GetActorLocation() + FVector(500, 0, 0);
    FRotator SpawnRotation = FRotator::ZeroRotator;*/

    FVector PlayerLocation = GetOuterAPlayerController()->GetPawn()->GetActorLocation();
    FVector Forward = GetOuterAPlayerController()->GetPawn()->GetActorForwardVector(); // 플레이어가 바라보는 방향

    FVector SpawnLocation = PlayerLocation + Forward * 500.0f;
    FVector DirectionToPlayer = (PlayerLocation - SpawnLocation).GetSafeNormal();
    FRotator SpawnRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();


    FString Key = MonsterName.ToLower();
    if (TSubclassOf<AActor>* FoundClass = MonsterMap.Find(Key))
    {
        World->SpawnActor<AActor>(*FoundClass, SpawnLocation, SpawnRotation);
        UE_LOG(LogTemp, Warning, TEXT("Spawned monster: %s"), *Key);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Monster not found for key: %s"), *Key);
    }
}
