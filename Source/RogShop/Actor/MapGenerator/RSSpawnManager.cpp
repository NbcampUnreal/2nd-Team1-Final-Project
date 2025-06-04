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
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"

#pragma region 초기화 함수

// 외부에서 전달받은 월드 및 테이블 초기화
void URSSpawnManager::Initialize(UWorld* InWorld, UGameInstance* GameInstance, TSubclassOf<AActor> ShopNPC)
{
	World = InWorld;
	ShopNPCClass = ShopNPC;
	
	if (!GameInstance) return;

	URSDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<URSDataSubsystem>();
	if (!DataSubsystem) return;

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
	
	// OnBossDead.AddDynamic(this, &URSSpawnManager::SpawnDunNextStagePortal);
}
#pragma endregion

#pragma region 몬스터 스폰

// 레벨 내 Monster 태그가 있는 TargetPoint 위치에 몬스터 스폰
void URSSpawnManager::SpawnMonstersInLevel()
{
	if (!World || !MonsterRawTable || !MonsterStateTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 스폰 실패"));
		return;
	}

	TArray<AActor*> MonsterTargets;
	for (TActorIterator<ATargetPoint> It(World); It; ++It)
	{
		if (It->Tags.Contains(FName("Monster")))
			MonsterTargets.Add(*It);
	}

	Algo::RandomShuffle(MonsterTargets);
	int32 TargetIndex = 0;

	TArray<FMonsterSpawnGroupData*> AllGroups;
	MonsterRawTable->GetAllRows(TEXT("MonsterRawData"), AllGroups);

	for (FMonsterSpawnGroupData* Group : AllGroups)
	{
		for (const FMonsterCount& Entry : Group->SpawnGroup)
		{
			FMonsterData* StateRow = MonsterStateTable->FindRow<FMonsterData>(Entry.MonsterRowName, TEXT("MonsterStateLookup"));
			if (!StateRow || !StateRow->MonsterClass) continue;

			for (int32 i = 0; i < Entry.Count && TargetIndex < MonsterTargets.Num(); ++i)
			{
				AActor* Target = MonsterTargets[TargetIndex++];
				FTransform SpawnTransform = Target->GetActorTransform();

				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				ARSDunMonsterCharacter* Monster = World->SpawnActor<ARSDunMonsterCharacter>(
					StateRow->MonsterClass, SpawnTransform, Params);

				// 충돌 캡슐 반영해 위치 미세 조정
				FVector SpawnLocation = SpawnTransform.GetLocation();
				SpawnLocation.Z += Monster->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
				Monster->SetActorLocation(SpawnLocation);

				Monster->IncreaseHP(StateRow->MaxHP);
				Monster->ChangeMoveSpeed(StateRow->MoveSpeed);
			}
		}
	}
}
#pragma endregion

#pragma region 상점 NPC 스폰

// NPC 태그가 있는 TargetPoint 중 하나에 상점 NPC 스폰
void URSSpawnManager::SpawnShopNPCInLevel()
{
	if (!World || !ShopNPCClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("상점 생성 실패"));
		return;
	}

	TArray<ATargetPoint*> ShopPoints;
	for (TActorIterator<ATargetPoint> It(World); It; ++It)
	{
		if (It->Tags.Contains(FName("NPC")))
			ShopPoints.Add(*It);
	}

	if (ShopPoints.Num() == 0) return;

	int32 Index = FMath::RandRange(0, ShopPoints.Num() - 1);
	ATargetPoint* ChosenPoint = ShopPoints[Index];

	FTransform SpawnTransform = ChosenPoint->GetActorTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	World->SpawnActor<AActor>(ShopNPCClass, SpawnTransform, SpawnParams);
}
#pragma endregion

#pragma region 플레이어 스폰

// Player 태그가 있는 TargetPoint에 플레이어 이동 또는 스폰
void URSSpawnManager::SpawnPlayerAtStartPoint(TSubclassOf<ACharacter> PlayerClass)
{
	if (!World || !PlayerClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed CreatePlayerCharacter"));
		return;
	}

	ATargetPoint* StartPoint = nullptr;
	for (TActorIterator<ATargetPoint> It(World); It; ++It)
	{
		if (It->Tags.Contains(FName("Player")) &&
			FVector::Dist(It->GetActorLocation(), FVector::ZeroVector) < 3000.f)
		{
			StartPoint = *It;
			break;
		}
	}

	if (!StartPoint) return;

	FTransform SpawnTransform = StartPoint->GetActorTransform();
	ACharacter* ExistingPlayer = UGameplayStatics::GetPlayerCharacter(World, 0);

	if (ExistingPlayer)
	{
		// 기존 플레이어 위치 이동
		ExistingPlayer->SetActorTransform(SpawnTransform);
	}
	else
	{
		// 새 플레이어 생성
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ACharacter* PlayerCharacter = World->SpawnActor<ACharacter>(PlayerClass, SpawnTransform, SpawnParams);

		FVector SpawnLocation = SpawnTransform.GetLocation();
		SpawnLocation.Z += PlayerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		PlayerCharacter->SetActorLocation(SpawnLocation);

		// 컨트롤러 연결
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
		if (PC) PC->Possess(PlayerCharacter);
	}
}
#pragma endregion