// Fill out your copyright notice in the Description page of Project Settings.


#include "RSSpawnManager.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "MonsterSpawnGroupData.h"
#include "MonsterData.h"
#include "DungeonObjectData.h"
#include "Algo/RandomShuffle.h"
#include "RSDungeonGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RSDunMonsterCharacter.h"
#include "RSDataSubsystem.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "RogShop/UtilDefine.h"

// 외부에서 전달받은 월드 및 테이블 초기화
void URSSpawnManager::Initialize(UWorld* InWorld, UGameInstance* GameInstance,int32 TileIndex)
{
	World = InWorld;
	LevelIndex = TileIndex;

	if (!GameInstance) return;

	URSDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<URSDataSubsystem>();
	if (!DataSubsystem)
	{
		return;
	}

	switch (LevelIndex)
	{
	case 0:
		// 숲 몬스터 그룹
		MonsterRawTable = DataSubsystem->ForestMonsterSpawnGroup;
		break;
	case 1:
		// 사막 몬스터 그룹
		MonsterRawTable = DataSubsystem->DesertMonsterSpawnGroup;
		break;
	case 2:
		// 동굴 몬스터 그룹
		MonsterRawTable = DataSubsystem->CaveMonsterSpawnGroup;
		break;
	default:
		return;
		break;
	}
	MonsterStateTable = DataSubsystem->Monster;
	DungeonObjectTable = DataSubsystem->DungeonObject;

	if (!MonsterRawTable)
	{
		RS_LOG_DEBUG("Failed Update MonsterRawTable");
		return;
	}
	if (!MonsterStateTable)
	{
		RS_LOG_DEBUG("Failed Update Monster");
		return;
	}
	if (!DungeonObjectTable)
	{
		RS_LOG_DEBUG("Failed Update DungeonObject");
		return;
	}
	
	if (AGameModeBase* GM = UGameplayStatics::GetGameMode(World))
	{
		if (ARSDungeonGameModeBase* DungeonGM = Cast<ARSDungeonGameModeBase>(GM))
		{
			DungeonGM->OnBossDead.AddDynamic(this, &URSSpawnManager::SpawnDunNextStagePortal);
		}
	}
}

// 레벨 내 Monster 태그가 있는 TargetPoint 위치에 몬스터 스폰
void URSSpawnManager::SpawnMonstersInLevel()
{
	if (!World || !MonsterRawTable || !MonsterStateTable) //월드나 테이블이 초기화되지 않았으면 리턴
	{
		RS_LOG_DEBUG("몬스터 스폰 실패: 월드 또는 테이블이 초기화되지 않음");
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("SpawnMonstersInLevel 시작됨"));

	TMap<FIntPoint, TArray<AActor*>> TileToTargets; // 타일별로 타겟 포인트들을 저장할 맵


	for (TActorIterator<ATargetPoint> It(World); It; ++It) // 월드 내의 모든 TargetPoint 중에서 Monster 태그를 가진 것들을 타일별로 분류
	{
		if (It->Tags.Contains(FName("Monster")))
		{
			ATargetPoint* Target = *It;
			FVector Location = Target->GetActorLocation();

			// 위치 기반 타일 좌표 계산 (맵 생성기 기준과 동일하게)
			int32 TileX = FMath::RoundToInt(Location.X / 4000.f); // 타일사이즈 4000.0f
			int32 TileY = FMath::RoundToInt(Location.Y / 4000.f);
			FIntPoint TileCoord(TileX, TileY);

			TileToTargets.FindOrAdd(TileCoord).Add(Target); //해당 타일 좌표에 타겟포인트 추가
		}
	}

	// 모든 스폰 그룹 데이터 테이블에서 가져오기
	TArray<FMonsterSpawnGroupData*> AllGroups;
	MonsterRawTable->GetAllRows(TEXT("MonsterRawData"), AllGroups);

	if (AllGroups.Num() == 0)
	{
		RS_LOG_DEBUG("MonsterRawTable에 데이터가 없습니다.");
		return;
	}

	int32 TotalSpawned = 0;

	// 각 타일마다 반복하면서 몬스터 스폰시도
	for (const TPair<FIntPoint, TArray<AActor*>>& Pair : TileToTargets)
	{
		const TArray<AActor*>& TilePoints = Pair.Value;

		// 타겟포인트가 없다면 경고로그 출력후 건너뜀
		if (TilePoints.Num() == 0)
		{
			RS_LOG_DEBUG("타일 %s 에 타겟포인트가 없습니다.", *Pair.Key.ToString());
			continue;
		}

		FMonsterSpawnGroupData* Group = AllGroups[FMath::RandRange(0, AllGroups.Num() - 1)];// 타일마다 그룹 하나를 랜덤으로 선택

		// 타겟 포인트 순서를 랜덤 섞기
		TArray<AActor*> ShuffledPoints = TilePoints;
		Algo::RandomShuffle(ShuffledPoints);
		int32 TargetIndex = 0;

		// 그룹 내에 정의된 몬스터 수 만큼 생성
		int TileSpawned = 0;
		for (const FMonsterCount& Entry : Group->SpawnGroup)
		{
			// 몬스터 상태 정보 테이블에서 해당 몬스터 정보 조회
			FMonsterData* StateRow = MonsterStateTable->FindRow<FMonsterData>(Entry.MonsterRowName, TEXT("MonsterStateLookup"));

			UE_LOG(LogTemp, Warning, TEXT("타일 (%d,%d) → 몬스터: %s x %d"),
				Pair.Key.X, Pair.Key.Y,
				*Entry.MonsterRowName.ToString(), Entry.Count);

			TileSpawned += Entry.Count;
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
					RS_LOG_DEBUG("스폰할 타겟포인트가 부족하여 몬스터 일부는 생략됩니다.");
					break;
				}

				// 타겟 포인트 순환적으로 사용
				AActor* Target = ShuffledPoints[TargetIndex % ShuffledPoints.Num()];
				TargetIndex++;
				//위치 약간 분산되게 조정
				float OffsetRadius = 100.f;
				float Angle = FMath::DegreesToRadians((i * 360.f) / Entry.Count);
				FVector Offset = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0) * OffsetRadius;
				FVector FinalLocation = Target->GetActorLocation() + Offset + FVector(0, 0, 50.f);

				FTransform SpawnTransform;
				SpawnTransform.SetLocation(FinalLocation);
				SpawnTransform.SetRotation(FQuat::Identity);

				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				// 몬스터 생성
				ARSDunMonsterCharacter* Monster = World->SpawnActor<ARSDunMonsterCharacter>(
					StateRow->MonsterClass, SpawnTransform, Params);

				//생성 실패시 로그 출력
				if (!Monster)
				{
					RS_LOG_DEBUG("몬스터 스폰 실패: %s", *StateRow->MonsterClass->GetName());
					continue;
				}

				// 충돌 캡슐 반영해 위치 미세 조정
				FVector SpawnLocation = SpawnTransform.GetLocation();
				SpawnLocation.Z += Monster->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
				Monster->SetActorLocation(SpawnLocation);

				//몬스터 능력치 초기화
				Monster->IncreaseHP(StateRow->MaxHP);
				Monster->ChangeMoveSpeed(StateRow->MoveSpeed);
				TotalSpawned++;
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("타일 (%d,%d) → 총 스폰 수: %d"), Pair.Key.X, Pair.Key.Y, TileSpawned);
	}
	UE_LOG(LogTemp, Warning, TEXT("총 스폰된 몬스터 수: %d"), TotalSpawned);
}

// NPC 태그가 있는 TargetPoint 중 하나에 상점 NPC 스폰
void URSSpawnManager::SpawnShopNPCInLevel()
{
	if (!World)
	{
		RS_LOG_DEBUG("상점 생성 실패");
		return;
	}

	TArray<ATargetPoint*> ShopPoints;
	for (TActorIterator<ATargetPoint> It(World); It; ++It)
	{
		if (It->Tags.Contains(FName("Shop")))
			ShopPoints.Add(*It);
	}

	if (ShopPoints.Num() == 0) return;

	int32 Index = FMath::RandRange(0, ShopPoints.Num() - 1);
	ATargetPoint* ChosenPoint = ShopPoints[Index];

	FTransform SpawnTransform = ChosenPoint->GetActorTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	World->SpawnActor<AActor>(DungeonObjectTable->FindRow<FDungeonObjectData>(FName("ShopNPC"), TEXT("SpawnActor"))->ObjectClass, SpawnTransform, SpawnParams);
	RS_LOG_DEBUG("상점 생성 성공");
}

// Player 태그가 있는 TargetPoint에 플레이어 이동 또는 스폰
void URSSpawnManager::SpawnPlayerAtStartPoint()
{
	if (!World)
	{
		RS_LOG_DEBUG("Failed CreatePlayerCharacter");
		return;
	}

	ATargetPoint* StartPoint = nullptr;
	for (TActorIterator<ATargetPoint> It(World); It; ++It)
	{
		ATargetPoint* Target = *It;

		// 모든 타겟포인트의 태그와 위치를 로그로 출력
		if (It->Tags.Contains(FName("Player")))
		{
			RS_LOG_DEBUG("TargetPoint 이름: %s, 위치: %s, 태그 수: %d", 
				*Target->GetName(), 
				*Target->GetActorLocation().ToString(),
				Target->Tags.Num());
		}


		if (It->Tags.Contains(FName("Player")))
		{
			FVector Location = It->GetActorLocation();
			if (Location.X >= -2000.f && Location.X <= 2000.f && Location.Y >= -2000.f && Location.Y <= 2000.f)
			{
				StartPoint = *It;
				break;
			}
		}
	}

	if (!StartPoint)
	{
		RS_LOG_DEBUG("스타트 포인트가 null입니다");
		return;
	}

	FTransform SpawnTransform = StartPoint->GetActorTransform();
	ACharacter* ExistingPlayer = UGameplayStatics::GetPlayerCharacter(World, 0);

	if (!ExistingPlayer)
	{
		RS_LOG_DEBUG("ExistingPlayer가 nullptr입니다.");
	}
	if (!World)
	{
		RS_LOG_DEBUG("World가 nullptr입니다.");
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

FVector URSSpawnManager::GetBossArenaLocation() const // 보스 아레나 위치를 찾는 함수
{
	if (!World)
	{
		RS_LOG_DEBUG("[보스아레나] 월드가 유효하지 않습니다.");
		return FVector::ZeroVector;
	}

	for (TActorIterator<ATargetPoint> It(World); It; ++It)
	{
		if (It->Tags.Contains(FName("BossArena")))
		{
			RS_LOG_DEBUG("BossArena 타겟포인트 발견: %s", *It->GetName());
			return It->GetActorLocation();
		}
	}

	RS_LOG_DEBUG("BossArena 태그가 있는 타겟포인트를 찾지 못했습니다.");
	return FVector::ZeroVector;
}

AActor* URSSpawnManager::SpawnBossPortal(const FVector& BossWorldLocation) // 보스 포탈을 찾는 함수
{
	if (!World)
	{
		RS_LOG_DEBUG("World가 유효하지 않아 BossPortalTarget 검색 실패");
		return nullptr;
	}

	const float TileHalfSize = 2000.f; // 타일 크기 4000이라 반지름 기준

	for (TActorIterator<ATargetPoint> It(World); It; ++It)
	{
		ATargetPoint* Target = *It;
		if (Target->Tags.Contains(FName("BossPotal")))
		{
			FVector TargetLocation = Target->GetActorLocation();

			// 타겟 위치가 보스룸 범위 내인지 확인
			if (FMath::Abs(TargetLocation.X - BossWorldLocation.X) <= TileHalfSize &&
				FMath::Abs(TargetLocation.Y - BossWorldLocation.Y) <= TileHalfSize)
			{
				RS_LOG_DEBUG("보스 포탈 타겟 발견: %s", *Target->GetName());
				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(BossWorldLocation);
				SpawnTransform.SetRotation(FQuat::Identity);
				ARSDunBossRoomPortal* SpawnedPortal = World->SpawnActor<ARSDunBossRoomPortal>(DungeonObjectTable->FindRow<FDungeonObjectData>(FName("BossRoomPortal"), TEXT("SpawnActor"))->ObjectClass, SpawnTransform, Params);
				RS_LOG_DEBUG("보스포탈 생성 완료");

				if (SpawnedPortal)
				{
					FTransform BossArenaTransform;
					FVector SpawnLocation = GetBossArenaLocation();
					ACharacter* ExistingPlayer = UGameplayStatics::GetPlayerCharacter(World, 0);
					float HalfHeight = 88.f;
					if (ExistingPlayer && ExistingPlayer->GetCapsuleComponent())
					{
						HalfHeight = ExistingPlayer->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
					}
					SpawnLocation.Z += HalfHeight;
					BossArenaTransform.SetLocation(SpawnLocation);
					BossArenaTransform.SetRotation(FQuat::Identity);
					SpawnedPortal->SetTargetTransform(BossArenaTransform);
					RS_LOG_DEBUG("보스 아레나 위치 지정 완료");
				}

				return Target;
			}
		}
	}

	RS_LOG_DEBUG("보스 타일 범위 내에서 BossArena 타겟을 찾지 못했습니다.");
	return nullptr;
}

FVector URSSpawnManager::GetNextStageLocation() const //다음 스테이지 포탈 위치 찾는 함수
{
	if (!World)
	{
		RS_LOG_DEBUG("월드가 유효하지 않습니다.");
		return FVector::ZeroVector;
	}

	for (TActorIterator<ATargetPoint> It(World); It; ++It)
	{
		if (It->Tags.Contains(FName("NextPotal")))
		{
			RS_LOG_DEBUG("NextPotal 타겟포인트 발견: %s", *It->GetName());
			return It->GetActorLocation();
		}
	}

	RS_LOG_DEBUG("NextPotal 태그가 있는 타겟포인트를 찾지 못했습니다.");
	return FVector::ZeroVector;
}



void URSSpawnManager::SpawnDunNextStagePortal() // 다음 스테이지 포탈 생성 함수
{
	if (!World)
	{
		RS_LOG_DEBUG("다음 스테이지 포탈 생성 실패: World 또는 PortalClass 누락");
		return;
	}

	FTransform PortalTransform;
	PortalTransform.SetLocation(GetNextStageLocation()); 

	World->SpawnActor<AActor>(DungeonObjectTable->FindRow<FDungeonObjectData>(FName("NextStagePortal"), TEXT("SpawnActor"))->ObjectClass, PortalTransform);
}

void URSSpawnManager::SpawnBossMonster()
{
	if (!World || !MonsterStateTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("보스 몬스터 스폰 실패: World 또는 MonsterStateTable이 초기화되지 않음"));
		return;
	}

	// Boss 태그가 붙은 TargetPoint 찾기
	ATargetPoint* BossTarget = nullptr;
	for (TActorIterator<ATargetPoint> It(World); It; ++It)
	{
		if (It->Tags.Contains(FName("Boss")))
		{
			BossTarget = *It;
			break;
		}
	}

	if (!BossTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss 태그를 가진 TargetPoint를 찾을 수 없습니다."));
		return;
	}
	FName BossMonsterRowName;
	// MonsterStateTable에서 보스몬스터 찾기
	switch (LevelIndex)
	{
	case 0:
		BossMonsterRowName=TEXT("MainFlower");
		break;
	case 1:
		BossMonsterRowName=TEXT("Worm");
		break;
	case 2:
		BossMonsterRowName=TEXT("SpiderQueen");
		break;
	default:
		BossMonsterRowName = NAME_None;
		break;
	}
	FMonsterData* BossData = MonsterStateTable->FindRow<FMonsterData>(BossMonsterRowName, TEXT("SpawnBossMonster"));
	if (!BossData || !BossData->MonsterClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterStateTable에서 MainFloor 보스 몬스터 정보를 찾을 수 없습니다."));
		return;
	}

	// 보스 스폰 위치 설정
	FVector SpawnLocation = BossTarget->GetActorLocation();
	FRotator SpawnRotation = BossTarget->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 보스 몬스터 생성
	ARSDunMonsterCharacter* BossMonster = World->SpawnActor<ARSDunMonsterCharacter>(
		BossData->MonsterClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (!BossMonster)
	{
		UE_LOG(LogTemp, Error, TEXT("보스 몬스터 스폰 실패: %s"), *BossData->MonsterClass->GetName());
		return;
	}

	// 위치 조정 (Z축 캡슐 반영)
	SpawnLocation.Z += BossMonster->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	BossMonster->SetActorLocation(SpawnLocation);

	// 능력치 설정
	BossMonster->IncreaseHP(BossData->MaxHP);
	BossMonster->ChangeMoveSpeed(BossData->MoveSpeed);

	UE_LOG(LogTemp, Warning, TEXT("보스 몬스터 '%s'가 스폰되었습니다."), *BossMonster->GetName());

}