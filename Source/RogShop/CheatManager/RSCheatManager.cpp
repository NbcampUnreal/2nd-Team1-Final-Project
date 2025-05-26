// Fill out your copyright notice in the Description page of Project Settings.


#include "RSCheatManager.h"
#include "RSDunMonsterCharacter.h"
#include "RSDunAnubisCharacter.h"
#include "RSDunBossSpiderQueenCharacter.h"
#include "RSDunPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

URSCheatManager::URSCheatManager()
{
    MonsterMap.Add("anubis", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunAnubisCharacter.BP_RSDunAnubisCharacter_C")));
    MonsterMap.Add("bossspiderqueen", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunBossSpiderQueenCharacter.BP_RSDunBossSpiderQueenCharacter_C")));
    MonsterMap.Add("boar", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunBoarCharacter.BP_RSDunBoarCharacter_C")));
    MonsterMap.Add("skeleton", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunSkeletonCharacter.BP_RSDunSkeletonCharacter_C")));
    MonsterMap.Add("bossflower", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunBossFlowerCharacter.BP_RSDunBossFlowerCharacter_C")));
    MonsterMap.Add("goat", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunGoatCharacter.BP_RSDunGoatCharacter_C")));
    MonsterMap.Add("chicken", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunChickenCharacter.BP_RSDunChickenCharacter_C")));
    MonsterMap.Add("elitebear", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSEliteBearCharacter.BP_RSEliteBearCharacter_C")));
    MonsterMap.Add("hound", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunUndeadHoundCharacter.BP_RSDunUndeadHoundCharacter_C")));
    MonsterMap.Add("snail", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunSnailCharacter.BP_RSDunSnailCharacter_C")));
    MonsterMap.Add("eliteicegolem", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunEliteIceGolemCharacter.BP_RSDunEliteIceGolemCharacter_C")));
    MonsterMap.Add("lich", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunEliteLichCharacter.BP_RSDunEliteLichCharacter_C")));
    MonsterMap.Add("bossdemon", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunFinalBossDemonCharacter.BP_RSDunFinalBossDemonCharacter_C")));
    MonsterMap.Add("bossworm", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunBossWormCharacter.BP_RSDunBossWormCharacter_C")));
    MonsterMap.Add("lizardman", LoadClass<AActor>(nullptr, TEXT("/Game/Blueprints/Characters/BP_RSDunLizardManCharacter.BP_RSDunLizardManCharacter_C")));
   
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
        //Monster->PlayDeathAnim();
        Monster->OnDeath();
    }
}

void URSCheatManager::SpawnMonster(FString MonsterName)
{
    UWorld* World = GetWorld();
    if (!World) return;

    FVector PlayerLocation = GetOuterAPlayerController()->GetPawn()->GetActorLocation();
    FVector Forward = GetOuterAPlayerController()->GetPawn()->GetActorForwardVector(); // �÷��̾ �ٶ󺸴� ����

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

void URSCheatManager::SpawnWeaponPad()
{
    if (!GetWorld() || !WeaponSpawnPadBPClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetWorld or WeaponSpawnPadBPClass Is Null !"));
        return;
    }

    APlayerController* PC = GetOuterAPlayerController();
    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("APlayerController Is Null !"));
        return;
    }

    APawn* PlayerPawn = PC->GetPawn();
    if (!PlayerPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("APawn Is Null !"));
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

void URSCheatManager::ShowRSDunMainWidget()
{
    ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(GetOuterAPlayerController());

    if (PC && PC->GetRSDunMainWidget())
    {
        PC->GetRSDunMainWidget()->AddToViewport();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PC or RSDunMainWidget Is Null !"));
    }
}

void URSCheatManager::OpenTycoonLevel()
{
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("TycoonTestMap"));
}
