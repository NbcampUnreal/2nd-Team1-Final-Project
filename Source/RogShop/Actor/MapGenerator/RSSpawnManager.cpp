// Fill out your copyright notice in the Description page of Project Settings.


#include "RSSpawnManager.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "MonsterSpawnGroupData.h"
#include "MonsterData.h"
#include "Algo/RandomShuffle.h"
#include "RSDungeonGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RSDunMonsterCharacter.h"
#include "RSDataSubsystem.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "RogShop/UtilDefine.h"
#include "RSDunBossRoomPortal.h" 
#include "RSDunNextStagePortal.h"
#include "RSDunLifeEssenceShop.h"
#include "ItemInfoData.h"
#include "RSDungeonGroundWeapon.h"
#include "RSDungeonGroundIngredient.h"
#include "RSDungeonGroundRelic.h"
#include "RSDungeonGroundLifeEssence.h"
#include "RSTileBlocker.h"

// 외부에서 전달받은 월드 및 테이블 초기화
void URSSpawnManager::Initialize(UWorld* InWorld, UGameInstance* GameInstance, int32 TargetLevelIndex)
{
	World = InWorld;
	LevelIndex = TargetLevelIndex;

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
		MonsterSpawnGroupDataTable = DataSubsystem->ForestMonsterSpawnGroup;
		break;
	case 1:
		// 사막 몬스터 그룹
		MonsterSpawnGroupDataTable = DataSubsystem->DesertMonsterSpawnGroup;
		break;
	case 2:
		// 동굴 몬스터 그룹
		MonsterSpawnGroupDataTable = DataSubsystem->CaveMonsterSpawnGroup;
		break;
	default:
		return;
		break;
	}
	MonsterDataTable = DataSubsystem->Monster;

	if (!MonsterSpawnGroupDataTable)
	{
		RS_LOG_DEBUG("Failed Update MonsterSpawnGroupDataTable");
		return;
	}
	if (!MonsterDataTable)
	{
		RS_LOG_DEBUG("Failed Update Monster");
		return;
	}
	
	if (AGameModeBase* GM = UGameplayStatics::GetGameMode(World))
	{
		if (ARSDungeonGameModeBase* DungeonGM = Cast<ARSDungeonGameModeBase>(GM))
		{
			DungeonGM->OnBossDead.AddDynamic(this, &URSSpawnManager::SpawnDunNextStagePortal);
			DungeonGM->OnBossRoomPortalEntered.AddDynamic(this, &URSSpawnManager::SpawnBossMonster);
		}
	}

	TileToTargets = BuildTileToTargets();
	RegisterAllTileBlockers();
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

	if (ShopPoints.Num() == 0)
	{
		return;
	}

	int32 Index = FMath::RandRange(0, ShopPoints.Num() - 1);
	ATargetPoint* ChosenPoint = ShopPoints[Index];

	FTransform SpawnTransform = ChosenPoint->GetActorTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	DunLifeEssenceShopInstance = World->SpawnActor<ARSDunLifeEssenceShop>(DunLifeEssenceShopClass, SpawnTransform, SpawnParams);

	RS_LOG_DEBUG("상점 생성 성공");
}

void URSSpawnManager::SpawnGroundWeaponAtTransform(FName TargetName, FTransform TargetTransform, bool AddImpulse)
{
	URSGameInstance* RSGameInstance = GetWorld()->GetGameInstance<URSGameInstance>();
	if (!RSGameInstance)
	{
		return;
	}

	URSDataSubsystem* DataSubsystem = RSGameInstance->GetSubsystem<URSDataSubsystem>();
	if (!DataSubsystem)
	{
		return;
	}

	FItemInfoData* WeaponData = DataSubsystem->WeaponInfo->FindRow<FItemInfoData>(TargetName, TEXT("Get ItemInfoData"));
	FDungeonWeaponData* WeaponClassData = DataSubsystem->WeaponDetail->FindRow<FDungeonWeaponData>(TargetName, TEXT("Get ItemInfoData"));

	if (WeaponData && WeaponClassData)
	{
		UStaticMesh* ItemStaticMesh = WeaponData->ItemStaticMesh;;
		TSubclassOf<ARSDungeonItemBase> WeaponClass = WeaponClassData->WeaponClass;

		if (ItemStaticMesh && WeaponClass)
		{
			ARSDungeonGroundWeapon* GroundWeapon = GetWorld()->SpawnActor<ARSDungeonGroundWeapon>(DungeonGroundWeaponClass, TargetTransform);

			FText ItemName = WeaponData->ItemName;

			if (GroundWeapon)
			{
				GroundWeapon->InitGroundItemInfo(ItemName, false, TargetName, ItemStaticMesh);
				GroundWeapon->SetWeaponClass(WeaponClassData->WeaponClass);

				if (AddImpulse)
				{
					GroundWeapon->RandImpulse();
				}
			}
		}
	}
}

void URSSpawnManager::SpawnGroundIngredientAtTransform(FName TargetName, FTransform TargetTransform, int32 Amount)
{
	URSGameInstance* RSGameInstance = GetWorld()->GetGameInstance<URSGameInstance>();
	if (!RSGameInstance)
	{
		return;
	}

	URSDataSubsystem* DataSubsystem = RSGameInstance->GetSubsystem<URSDataSubsystem>();
	if (!DataSubsystem)
	{
		return;
	}

	UDataTable* IngredientInfoDataTable = DataSubsystem->IngredientInfo;
	if (!IngredientInfoDataTable)
	{
		RS_LOG("재료 데이터테이블 nullptr");
		return;
	}

	if (!MonsterDataTable)
	{
		RS_LOG("캐싱 된 데이터 테이블이 nullptr");
		return;
	}

	FItemInfoData* IngredientInfoDataRow = IngredientInfoDataTable->FindRow<FItemInfoData>(TargetName, TEXT("Get IngredientDetailData"));
	if (IngredientInfoDataRow)
	{
		ARSDungeonGroundIngredient* DungeonIngredient = GetWorld()->SpawnActor<ARSDungeonGroundIngredient>(DungeonGroundIngredientClass, TargetTransform);

		if (DungeonIngredient)
		{
			FText ItemName = IngredientInfoDataRow->ItemName;
			UStaticMesh* ItemStaticMesh = IngredientInfoDataRow->ItemStaticMesh;

			DungeonIngredient->InitGroundItemInfo(ItemName, false, TargetName, ItemStaticMesh);
			DungeonIngredient->SetQuantity(Amount);
			DungeonIngredient->RandImpulse();
		}
	}
}

void URSSpawnManager::SpawnGroundRelicAtTransform(FName TargetName, FTransform TargetTransform)
{
	UGameInstance* CurGameInstance = GetWorld()->GetGameInstance();
	if (!CurGameInstance)
	{
		return;
	}

	URSDataSubsystem* DataSubsystem = CurGameInstance->GetSubsystem<URSDataSubsystem>();
	if (!DataSubsystem)
	{
		return;
	}

	UDataTable* RelicInfoDataTable = DataSubsystem->RelicInfo;
	UDataTable* RelicClassDataTable = DataSubsystem->RelicDetail;
	if (!RelicInfoDataTable || !RelicClassDataTable)
	{
		return;
	}

	FItemInfoData* RelicInfoDataRow = RelicInfoDataTable->FindRow<FItemInfoData>(TargetName, TEXT("Get ItemInfoData"));
	FDungeonRelicData* RelicClassDataRow = RelicClassDataTable->FindRow<FDungeonRelicData>(TargetName, TEXT("Get RelicClassData"));
	if (RelicInfoDataRow && RelicClassDataRow)
	{
		ARSDungeonGroundRelic* GroundRelic = GetWorld()->SpawnActor<ARSDungeonGroundRelic>(DungeonGroundRelicClass, TargetTransform);
		
		if (GroundRelic)
		{
			FText ItemName = RelicInfoDataRow->ItemName;
			UStaticMesh* ItemStaticMesh = RelicInfoDataRow->ItemStaticMesh;

			GroundRelic->InitGroundItemInfo(ItemName, false, TargetName, ItemStaticMesh);
			GroundRelic->SetRelicClass(RelicClassDataRow->RelicClass);
		}
	}
}

void URSSpawnManager::SpawnGroundIngredientFromCharacter(ARSDunBaseCharacter* DiedCharacter)
{
	ARSDunMonsterCharacter* MonsterCharacter = Cast<ARSDunMonsterCharacter>(DiedCharacter);
	FName MonsterRowName = NAME_None;
	if (MonsterCharacter)
	{
		MonsterRowName = MonsterCharacter->GetMonsterRowName();
	}

	if (MonsterRowName.IsNone())
	{
		return;
	}

	UGameInstance* CurGameInstance = MonsterCharacter->GetGameInstance();
	if (!CurGameInstance)
	{
		return;
	}

	URSDataSubsystem* DataSubsystem = CurGameInstance->GetSubsystem<URSDataSubsystem>();
	if (!DataSubsystem)
	{
		return;
	}

	UDataTable* IngredientInfoDataTable = DataSubsystem->IngredientInfo;
	if (!IngredientInfoDataTable)
	{
		RS_LOG("재료 데이터테이블 nullptr");
		return;
	}

	if (!MonsterDataTable)
	{
		RS_LOG("캐싱 된 데이터 테이블이 nullptr");
		return;
	}

	FMonsterData* MonsterDataRow = MonsterDataTable->FindRow<FMonsterData>(MonsterRowName, TEXT("Get MonsterDataRow"));

	if (MonsterDataRow && MonsterDataRow->Ingredients.Num() >= 0)
	{
		for (const FMonsterIngredientsData& e : MonsterDataRow->Ingredients)
		{
			// TODO : 드랍 확률 적용하기

			FItemInfoData* IngredientInfoDataRow = IngredientInfoDataTable->FindRow<FItemInfoData>(e.IngredientName, TEXT("Get IngredientDetailData"));
			if (IngredientInfoDataRow)
			{
				ARSDungeonGroundIngredient* DungeonIngredient = GetWorld()->SpawnActor<ARSDungeonGroundIngredient>(DungeonGroundIngredientClass, MonsterCharacter->GetTransform());

				if (DungeonIngredient)
				{
					FText ItemName = IngredientInfoDataRow->ItemName;
					UStaticMesh* ItemStaticMesh = IngredientInfoDataRow->ItemStaticMesh;

					DungeonIngredient->InitGroundItemInfo(ItemName, false, e.IngredientName, ItemStaticMesh);
					DungeonIngredient->SetQuantity(1);
					DungeonIngredient->RandImpulse();
				}
			}
		}
	}
}

void URSSpawnManager::SpawnGroundLifeEssenceFromCharacter(ARSDunBaseCharacter* DiedCharacter)
{
	if (!MonsterDataTable)
	{
		RS_LOG("캐싱 된 데이터 테이블이 nullptr");
		return;
	}

	ARSDunMonsterCharacter* MonsterCharacter = Cast<ARSDunMonsterCharacter>(DiedCharacter);
	FName MonsterRowName = NAME_None;
	if (MonsterCharacter)
	{
		MonsterRowName = MonsterCharacter->GetMonsterRowName();
	}

	if (MonsterRowName.IsNone())
	{
		return;
	}

	FMonsterData* MonsterDataRow = MonsterDataTable->FindRow<FMonsterData>(MonsterRowName, TEXT("Get MonsterDataRow"));

	if (MonsterDataRow)
	{
		ARSDungeonGroundLifeEssence* DungeonLifeEssence = GetWorld()->SpawnActor<ARSDungeonGroundLifeEssence>(DungeonGroundLifeEssenceClass, MonsterCharacter->GetTransform());

		if (DungeonLifeEssence)
		{
			int32 LifeEssenceQuantity = MonsterDataRow->DropLifeEssenceQuantity;

			DungeonLifeEssence->RandImpulse();
			DungeonLifeEssence->SetQuantity(LifeEssenceQuantity);
		}
	}
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

				DunBossRoomPortalInstance = World->SpawnActor<ARSDunBossRoomPortal>(DunBossRoomPortalClass, SpawnTransform, Params);
				RS_LOG_DEBUG("보스포탈 생성 완료");

				if (DunBossRoomPortalInstance)
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
					DunBossRoomPortalInstance->SetTargetTransform(BossArenaTransform);
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
	if (!World || !DunNextStagePortalClass)
	{
		RS_LOG_DEBUG("다음 스테이지 포탈 생성 실패: World 또는 PortalClass 누락");
		return;
	}

	FTransform PortalTransform;
	PortalTransform.SetLocation(GetNextStageLocation()); 

	DunNextStagePortalInstance = World->SpawnActor<ARSDunNextStagePortal>(DunNextStagePortalClass, PortalTransform);
}

void URSSpawnManager::SpawnBossMonster()
{
	if (!World || !MonsterDataTable)
	{
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
	FMonsterData* BossData = MonsterDataTable->FindRow<FMonsterData>(BossMonsterRowName, TEXT("SpawnBossMonster"));
	if (!BossData || !BossData->MonsterClass)
	{
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
		return;
	}

	// 위치 조정 (Z축 캡슐 반영)
	SpawnLocation.Z += BossMonster->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	BossMonster->SetActorLocation(SpawnLocation);

	// 능력치 설정
	BossMonster->IncreaseHP(BossData->MaxHP);
	BossMonster->ChangeMoveSpeed(BossData->MoveSpeed);

	// 사망 시 오브젝트 스폰 함수 바인딩
	BossMonster->OnCharacterDied.AddDynamic(this, &URSSpawnManager::SpawnGroundIngredientFromCharacter);
	BossMonster->OnCharacterDied.AddDynamic(this, &URSSpawnManager::SpawnGroundLifeEssenceFromCharacter);

}

void URSSpawnManager::SpawnMonstersAtTile(FIntPoint TileCoord) // 특정 타일 좌표에 몬스터 스폰하는 함수
{
	if (!World || !MonsterSpawnGroupDataTable || !MonsterDataTable)
	{
		return;
	}

	if (SpawnedTiles.Contains(TileCoord) || ClearedTiles.FindRef(TileCoord)) // 이미 스폰된 타일이거나 클리어된 타일이면 생략
	{
		return;
	}

	const TArray<AActor*>* Targets = TileToTargets.Find(TileCoord); // 타일 좌표에 해당하는 타겟 포인트 배열 찾기
	if (!Targets || Targets->Num() == 0) 
	{
		return;
	}

	TArray<FMonsterSpawnGroupData*> AllGroups; // 모든 몬스터 스폰 그룹 데이터 테이블에서 가져오기
	MonsterSpawnGroupDataTable->GetAllRows(TEXT("MonsterData"), AllGroups);
	if (AllGroups.Num() == 0)
	{
		return;
	}

	FMonsterSpawnGroupData* Group = AllGroups[FMath::RandRange(0, AllGroups.Num() - 1)]; // 랜덤으로 그룹 선택
	TArray<AActor*> ShuffledPoints = *Targets; // 해당 타일 좌표의 타겟 포인트 배열을 복사
	Algo::RandomShuffle(ShuffledPoints); // 타겟 포인트 순서를 랜덤 섞기

	int32 TargetIndex = 0;
	int32 TotalSpawned = 0;

	for (const FMonsterCount& Entry : Group->SpawnGroup) // 그룹 내에 정의된 몬스터 수 만큼 생성
	{
		FMonsterData* StateRow = MonsterDataTable->FindRow<FMonsterData>(Entry.MonsterRowName, TEXT("MonsterState")); 
		if (!StateRow || !StateRow->MonsterClass)
		{
			continue;
		}

		for (int32 i = 0; i < Entry.Count; ++i)
		{
			if (ShuffledPoints.Num() == 0)
			{
				break;
			}

			AActor* Target = ShuffledPoints[TargetIndex % ShuffledPoints.Num()]; 
			TargetIndex++; 

			FVector FinalLoc = Target->GetActorLocation() + FVector(FMath::FRandRange(-100.0f, 100.0f), FMath::FRandRange(-100.0f, 100.0f), 50); // 위치 약간 분산되게 조정
			FTransform SpawnTransform(FinalLoc); 
			FActorSpawnParameters Params; 
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			ARSDunMonsterCharacter* Monster = World->SpawnActor<ARSDunMonsterCharacter>(StateRow->MonsterClass, SpawnTransform, Params);
			if (!Monster)
			{
				continue;
			}

			Monster->IncreaseHP(StateRow->MaxHP); 
			Monster->ChangeMoveSpeed(StateRow->MoveSpeed); 
			Monster->OnCharacterDied.AddDynamic(this, &URSSpawnManager::OnMonsterDiedFromTile); // 몬스터 사망 시 호출될 델리게이트 바인딩

			// 사망 시 오브젝트 스폰 함수 바인딩
			Monster->OnCharacterDied.AddDynamic(this, &URSSpawnManager::SpawnGroundIngredientFromCharacter);
			Monster->OnCharacterDied.AddDynamic(this, &URSSpawnManager::SpawnGroundLifeEssenceFromCharacter);

			AliveMonstersPerTile.FindOrAdd(TileCoord) += 1;
			TotalSpawned++;
		}
	}

	if (TotalSpawned > 0) // 타일에 몬스터가 스폰되었다면
	{
		SpawnedTiles.Add(TileCoord); // 몬스터가 스폰된 타일로 등록
	}

	if (TileToBlockers.Contains(TileCoord)) // 타일 좌표에 해당하는 블로커가 있다면
	{
		for (ARSTileBlocker* Blocker : TileToBlockers[TileCoord]) 
		{
			if (Blocker)
			{
				Blocker->RaiseBlocker(); // 블로커를 올린다
			}
		}
	}
}

void URSSpawnManager::OnMonsterDiedFromTile(ARSDunBaseCharacter* DiedCharacter) // 몬스터가 타일에서 죽었을 때 호출되는 함수
{
	if (!DiedCharacter)
	{
		return;
	}

	FVector Loc = DiedCharacter->GetActorLocation(); 
	FIntPoint TileCoord = FIntPoint(FMath::RoundToInt(Loc.X / 4000.f), FMath::RoundToInt(Loc.Y / 4000.f)); 

	if (!AliveMonstersPerTile.Contains(TileCoord))
	{
		return;
	}

	AliveMonstersPerTile[TileCoord]--; // 해당 타일의 몬스터 수 감소

	if (AliveMonstersPerTile[TileCoord] <= 0) // 몬스터가 모두 죽었을 때
	{
		ClearedTiles.Add(TileCoord, true); // 타일 클리어 상태로 등록

		if (TileToBlockers.Contains(TileCoord)) //블로커를 내린다
		{
			for (ARSTileBlocker* Blocker : TileToBlockers[TileCoord])
			{
				if (Blocker)
				{
					Blocker->LowerBlocker();
				}
			}
		}

		if (TileCoord == BossTileCoord)
		{
			SpawnBossPortal(BossPotalLoaction);
		}
		// TODO: 타일 클리어시 실행할 이벤트 처리
	}
}

TMap<FIntPoint, TArray<AActor*>> URSSpawnManager::BuildTileToTargets() // 타일 좌표별로 타겟 포인트를 매핑하는 함수
{
	TMap<FIntPoint, TArray<AActor*>> Result; // 타일 좌표와 타겟 포인트 배열을 매핑할 맵

	if (!World)
	{
		return Result;
	}

	for (TActorIterator<ATargetPoint> It(World); It; ++It)
	{
		if (It->Tags.Contains(FName("Monster")))
		{
			FVector Location = It->GetActorLocation();

			// 타일 위치 좌표화 (4000단위 기준)
			int32 TileX = FMath::RoundToInt(Location.X / 4000.f); // 4000 단위로 나누어 타일 좌표 계산
			int32 TileY = FMath::RoundToInt(Location.Y / 4000.f); // 4000 단위로 나누어 타일 좌표 계산
			FIntPoint TileCoord(TileX, TileY);

			Result.FindOrAdd(TileCoord).Add(*It);
		}
	}

	return Result;
}

void URSSpawnManager::SetBossTileCoord(const FVector& BossWorldLocation)
{
	int32 TileX = FMath::RoundToInt(BossWorldLocation.X / 4000.f);
	int32 TileY = FMath::RoundToInt(BossWorldLocation.Y / 4000.f);
	BossPotalLoaction = BossWorldLocation;
	BossTileCoord = FIntPoint(TileX, TileY);
}

void URSSpawnManager::RegisterAllTileBlockers()
{
	if (!World)
	{
		return;
	}

	TileToBlockers.Empty(); // 중복 방지용

	for (TActorIterator<ARSTileBlocker> It(World); It; ++It)
	{
		ARSTileBlocker* Blocker = *It;
		if (Blocker)
		{
			FIntPoint Coord = Blocker->GetLinkedTileCoord();
			TileToBlockers.FindOrAdd(Coord).Add(Blocker);
		}
	}
}
