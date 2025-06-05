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

// 레벨 내 Monster 태그가 있는 TargetPoint 위치에 몬스터 스폰
void URSSpawnManager::SpawnMonstersInLevel()
{
	if (!World || !MonsterRawTable || !MonsterStateTable) //월드나 테이블이 초기화되지 않았으면 리턴
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 스폰 실패: 월드 또는 테이블이 초기화되지 않음"));
		return;
	}

	TMap<FVector2D, TArray<AActor*>> TileToTargets; // 타일별로 타겟 포인트들을 저장할 맵


	for (TActorIterator<ATargetPoint> It(World); It; ++It) // 월드 내의 모든 TargetPoint 중에서 Monster 태그를 가진 것들을 타일별로 분류
	{
		if (It->Tags.Contains(FName("Monster")))
		{
			ATargetPoint* Target = *It;
			FVector Location = Target->GetActorLocation();

			// 위치 기반 타일 좌표 계산 (맵 생성기 기준과 동일하게)
			int32 TileX = FMath::FloorToInt(Location.X / 4000.f); // 타일사이즈 4000.0f
			int32 TileY = FMath::FloorToInt(Location.Y / 4000.f);
			FVector2D TileCoord(TileX, TileY);

			TileToTargets.FindOrAdd(TileCoord).Add(Target); //해당 타일 좌표에 타겟포인트 추가
		}
	}

	// 모든 스폰 그룹 데이터 테이블에서 가져오기
	TArray<FMonsterSpawnGroupData*> AllGroups;
	MonsterRawTable->GetAllRows(TEXT("MonsterRawData"), AllGroups);

	if (AllGroups.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterRawTable에 데이터가 없습니다."));
		return;
	}

	int32 GroupIndex = 0; // 그룹 인덱스 초기화

	// 각 타일마다 반복하면서 몬스터 스폰시도
	for (const TPair<FVector2D, TArray<AActor*>>& Pair : TileToTargets)
	{
		const TArray<AActor*>& TilePoints = Pair.Value;

		// 타겟포인트가 없다면 경고로그 출력후 건너뜀
		if (TilePoints.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("타일 %s 에 타겟포인트가 없습니다."), *Pair.Key.ToString());
			continue;
		}

		// 스폰 그룹을 순차적으로 선택
		FMonsterSpawnGroupData* Group = AllGroups[GroupIndex % AllGroups.Num()];
		GroupIndex++;

		// 타겟 포인트 순서를 랜덤 섞기
		TArray<AActor*> ShuffledPoints = TilePoints;
		Algo::RandomShuffle(ShuffledPoints);
		int32 TargetIndex = 0;

		// 그룹 내에 정의된 몬스터 수 만큼 생성
		for (const FMonsterCount& Entry : Group->SpawnGroup)
		{
			// 몬스터 상태 정보 테이블에서 해당 몬스터 정보 조회
			FMonsterData* StateRow = MonsterStateTable->FindRow<FMonsterData>(Entry.MonsterRowName, TEXT("MonsterStateLookup"));
			if (!StateRow || !StateRow->MonsterClass)
			{
				continue;
			}

			// 몬스터 수 만큼 반복 생성
			for (int32 i = 0; i < Entry.Count; ++i)
			{
				// 타겟포인트가 비어있다면 스폰 일부 생략
				if (ShuffledPoints.Num() == 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("스폰할 타겟포인트가 부족하여 몬스터 일부는 생략됩니다."));
					break;
				}

				// 타겟 포인트 순환적으로 사용
				AActor* Target = ShuffledPoints[TargetIndex % ShuffledPoints.Num()];
				TargetIndex++;
				FTransform SpawnTransform = Target->GetActorTransform(); // 위치 정보

				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// 몬스터 생성
				ARSDunMonsterCharacter* Monster = World->SpawnActor<ARSDunMonsterCharacter>(
					StateRow->MonsterClass, SpawnTransform, Params);

				//생성 실패시 로그 출력
				if (!Monster)
				{
					UE_LOG(LogTemp, Error, TEXT("몬스터 스폰 실패: %s"), *StateRow->MonsterClass->GetName());
					continue;
				}

				// 충돌 캡슐 반영해 위치 미세 조정
				FVector SpawnLocation = SpawnTransform.GetLocation();
				SpawnLocation.Z += Monster->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
				Monster->SetActorLocation(SpawnLocation);

				//몬스터 능력치 초기화
				Monster->IncreaseHP(StateRow->MaxHP);
				Monster->ChangeMoveSpeed(StateRow->MoveSpeed);
			}
		}
	}
}

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
		ATargetPoint* Target = *It;

		// 모든 타겟포인트의 태그와 위치를 로그로 출력
		if (It->Tags.Contains(FName("Player")))
		{
			UE_LOG(LogTemp, Warning, TEXT("TargetPoint 이름: %s, 위치: %s, 태그 수: %d"),
				*Target->GetName(),
				*Target->GetActorLocation().ToString(),
				Target->Tags.Num());
		}


		if (It->Tags.Contains(FName("Player")) && FVector::Dist(It->GetActorLocation(), FVector::ZeroVector) < 3000.f)
		{
			StartPoint = *It;
			break;
		}
	}

	if (!StartPoint)
	{
		UE_LOG(LogTemp, Warning, TEXT("스타트 포인트가 null입니다"));
		return;
	}

	FTransform SpawnTransform = StartPoint->GetActorTransform();
	ACharacter* ExistingPlayer = UGameplayStatics::GetPlayerCharacter(World, 0);

	if (!ExistingPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("ExistingPlayer가 nullptr입니다."));
	}
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World가 nullptr입니다."));
	}

	if (ExistingPlayer)
	{
		FVector SpawnLocation = SpawnTransform.GetLocation();
		SpawnLocation.Z += ExistingPlayer->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		ExistingPlayer->SetActorLocation(SpawnLocation);
		ExistingPlayer->SetActorHiddenInGame(false);
		ExistingPlayer->SetActorEnableCollision(true);

		if (UCharacterMovementComponent* MoveComp = ExistingPlayer->GetCharacterMovement())
		{
			MoveComp->SetMovementMode(MOVE_Walking);
			MoveComp->GravityScale = 1.0f;
		}
	}
}

FVector URSSpawnManager::GetBossArenaLocation() const
{
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("월드가 유효하지 않습니다."));
		return FVector::ZeroVector;
	}

	for (TActorIterator<ATargetPoint> It(World); It; ++It)
	{
		if (It->Tags.Contains(FName("BossArena")))
		{
			UE_LOG(LogTemp, Log, TEXT("BossArena 타겟포인트 발견: %s"), *It->GetName());
			return It->GetActorLocation();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("BossArena 태그가 있는 타겟포인트를 찾지 못했습니다."));
	return FVector::ZeroVector;
}