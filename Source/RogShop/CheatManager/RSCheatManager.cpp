// Fill out your copyright notice in the Description page of Project Settings.


#include "RSCheatManager.h"
#include "RSDunMonsterCharacter.h"
#include "RSDunPlayerController.h"
#include "RSGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"

URSCheatManager::URSCheatManager()
{
#if WITH_EDITOR
    MonsterMap.Add("anubis", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunEliteAnubisCharacter.BP_DunEliteAnubisCharacter_C")));
    MonsterMap.Add("bossspiderqueen", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunBossSpiderQueenCharacter.BP_DunBossSpiderQueenCharacter_C")));
    MonsterMap.Add("boar", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunBoarCharacter.BP_DunBoarCharacter_C")));
    MonsterMap.Add("skeleton", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunSkeletonCharacter.BP_DunSkeletonCharacter_C")));
    MonsterMap.Add("bossflower", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunBossFlowerCharacter.BP_DunBossFlowerCharacter_C")));
    MonsterMap.Add("goat", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunGoatCharacter.BP_DunGoatCharacter_C")));
    MonsterMap.Add("chicken", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunChickenCharacter.BP_DunChickenCharacter_C")));
    MonsterMap.Add("elitebear", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunEliteBearCharacter.BP_DunEliteBearCharacter_C")));
    MonsterMap.Add("hound", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunUndeadHoundCharacter.BP_DunUndeadHoundCharacter_C")));
    MonsterMap.Add("snail", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunSnailCharacter.BP_DunSnailCharacter_C")));
    MonsterMap.Add("eliteicegolem", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunEliteIceGolemCharacter.BP_DunEliteIceGolemCharacter_C")));
    MonsterMap.Add("lich", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunEliteLichCharacter.BP_DunEliteLichCharacter_C")));
    MonsterMap.Add("bossdemon", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunFinalBossDemonCharacter.BP_DunFinalBossDemonCharacter_C")));
    MonsterMap.Add("bossworm", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunBossWormCharacter.BP_DunBossWormCharacter_C")));
    MonsterMap.Add("lizardman", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_DunLizardManCharacter.BP_DunLizardManCharacter_C")));
#endif

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
        Monster->PlayAttackAnim();
    }
}

void URSCheatManager::TestDunMonsterDeath()
{
    RS_LOG_DEBUG("1");
    UWorld* World = GetWorld();
    if (!World)
    {
        RS_LOG_DEBUG("2");
        return;
    }

    RS_LOG_DEBUG("3");
    ARSDunMonsterCharacter* Monster = Cast<ARSDunMonsterCharacter>(
        UGameplayStatics::GetActorOfClass(World, ARSDunMonsterCharacter::StaticClass())
    );

    if (Monster)
    {
        RS_LOG_DEBUG("4");
        //Monster->PlayDeathAnim();
        Monster->OnDeath();
    }
}

void URSCheatManager::SpawnMonster(FString MonsterName)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    FVector PlayerLocation = GetOuterAPlayerController()->GetPawn()->GetActorLocation();
    FVector Forward = GetOuterAPlayerController()->GetPawn()->GetActorForwardVector(); // �÷��̾ �ٶ󺸴� ����

    FVector SpawnLocation = PlayerLocation + Forward * 500.0f;
    FVector DirectionToPlayer = (PlayerLocation - SpawnLocation).GetSafeNormal();
    FRotator SpawnRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();

    FString Key = MonsterName.ToLower();
    if (TSubclassOf<AActor>* FoundClass = MonsterMap.Find(Key))
    {
        World->SpawnActor<AActor>(*FoundClass, SpawnLocation, SpawnRotation);
        RS_LOG_DEBUG("Spawned monster: %s", *Key);
    }
    else
    {
        RS_LOG_DEBUG("Monster not found for key: %s", *Key);
    }
}

void URSCheatManager::SpawnDunShopNPC()
{
    UWorld* World = GetWorld();

    if (!World)
    {
        return;
    }

    APlayerController* PC = GetOuterAPlayerController();

    if (!PC)
    {
        return;
    }

    if (!DunShopNPCClass)
    {
        return;
    }

    FVector SpawnLocation = PC->GetPawn()->GetActorLocation() + PC->GetPawn()->GetActorForwardVector() * 200.f;
    FActorSpawnParameters Params;
    Params.Owner = PC;

    World->SpawnActor<AActor>(DunShopNPCClass, SpawnLocation, FRotator::ZeroRotator, Params);
}

void URSCheatManager::SpawnWeaponPad()
{
    if (!GetWorld() || !WeaponSpawnPadBPClass)
    {
        return;
    }

    APlayerController* PC = GetOuterAPlayerController();
    if (!PC)
    {
        return;
    }

    APawn* PlayerPawn = PC->GetPawn();
    if (!PlayerPawn)
    {
        return;
    }

    FVector PlayerLocation = PlayerPawn->GetActorLocation();
    FRotator PlayerRotation = PlayerPawn->GetActorRotation();

    // 플레이어 앞 방향 벡터 (정면 방향)
    FVector ForwardVector = PlayerRotation.Vector();

    // 플레이어 앞 200 유닛 지점에 스폰
    FVector SpawnLocation = PlayerLocation + ForwardVector * 200.f;

    FRotator SpawnRotation = FRotator::ZeroRotator;

    GetWorld()->SpawnActor<AActor>(WeaponSpawnPadBPClass, SpawnLocation, SpawnRotation);
}

void URSCheatManager::ToggleDebugLog() const
{
    if (!GetWorld())
    {
        return;
    }

    URSGameInstance* RSGameInstance = Cast<URSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (!RSGameInstance)
    {
        return;
    }

    RSGameInstance->SetDebugLogEnabled(!RSGameInstance->GetDebugLogEnabled());

    RS_LOG_DEBUG("DebugLog is now: Enabled");
}

void URSCheatManager::OpenTycoonLevel()
{
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("TycoonTestMap"));
}
