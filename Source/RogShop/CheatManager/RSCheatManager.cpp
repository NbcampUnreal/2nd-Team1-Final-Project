// Fill out your copyright notice in the Description page of Project Settings.


#include "RSCheatManager.h"
#include "RSDunMonsterCharacter.h"
#include "Kismet/GameplayStatics.h"

void URSCheatManager::TestAnubisAttack()
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
        Monster->PlayBaseAttackAnim();
    }
}

void URSCheatManager::TestAnubisHItReact()
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
        Monster->PlayHitReactAnim();
    }
}

void URSCheatManager::TestAnubisDeath()
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

void URSCheatManager::SpawnDunShopNPC()
{
    UWorld* World = GetWorld();
    if (!World) return;

    APlayerController* PC = GetOuterAPlayerController();
    if (!PC) return;

    FVector SpawnLocation = PC->GetPawn()->GetActorLocation() + PC->GetPawn()->GetActorForwardVector() * 200.f;
    FActorSpawnParameters Params;
    Params.Owner = PC;

    TSubclassOf<AActor> NPCClass = LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Actor/DungeonNpc/BP_RSDunShopNpcActor.BP_RSDunShopNpcActor_C"));

    if (NPCClass)
    {
        World->SpawnActor<AActor>(NPCClass, SpawnLocation, FRotator::ZeroRotator, Params);
    }
}
