// Fill out your copyright notice in the Description page of Project Settings.


#include "RSPlayerWeaponComponent.h"
#include "RogShop/UtilDefine.h"
#include "RSBaseWeapon.h"
#include "RSDunPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "RSDungeonGroundWeapon.h"
#include "RSDunPlayerController.h"
#include "RSSaveGameSubsystem.h"
#include "RSDungeonWeaponSaveGame.h"

// Sets default values for this component's properties
URSPlayerWeaponComponent::URSPlayerWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	WeaponSlotSize = 2;
	WeaponActors.SetNum(WeaponSlotSize);
	WeaponSlot = EWeaponSlot::NONE;
}


// Called when the game starts
void URSPlayerWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadRequested();
	
	ARSDunPlayerCharacter* OwnerCharacter = GetOwner<ARSDunPlayerCharacter>();
	if (!OwnerCharacter)
	{
		return;
	}
	UGameInstance* CurGameInstance = OwnerCharacter->GetGameInstance();
	if (!CurGameInstance)
	{
		return;
	}

	URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
	if (!SaveGameSubsystem)
	{
		return;
	}

	SaveGameSubsystem->OnSaveRequested.AddDynamic(this, &URSPlayerWeaponComponent::SaveRequested);

}

void URSPlayerWeaponComponent::HandleNormalAttackInput()
{
    if (bIsAttack)
    {
        bComboInputBuffered = true;
    }
    else
    {
		int8 Index = static_cast<int8>(WeaponSlot) - 1;
        if (WeaponActors.IsValidIndex(Index) && WeaponActors[Index] != nullptr)
        {
			ARSDunPlayerCharacter* CurCharacter = GetOwner<ARSDunPlayerCharacter>();
            if (!CurCharacter)
            {
                return;
            }

            USkeletalMeshComponent* SkeletalMeshComp = CurCharacter->GetMesh();
            if (!SkeletalMeshComp)
            {
                return;
            }
            
            UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
            UAnimMontage* CurAttackMontage = WeaponActors[Index]->GetNormalAttack(ComboIndex);
            
            if (CurAttackMontage && AnimInstance)
            {
				float AttackSpeed = CurCharacter->GetAttackSpeed();

                AnimInstance->Montage_Play(CurAttackMontage, AttackSpeed);
                bIsAttack = true;
                ++ComboIndex;
            }
        }
    }
}

bool URSPlayerWeaponComponent::ContinueComboAttack()
{
	if (bComboInputBuffered)
	{
		int8 Index = static_cast<int8>(WeaponSlot) - 1;
		if (WeaponActors.IsValidIndex(Index) && WeaponActors[Index] != nullptr)
		{
			ARSDunPlayerCharacter* CurCharacter = GetOwner<ARSDunPlayerCharacter>();
			if (!CurCharacter)
			{
				return false;
			}

			USkeletalMeshComponent* SkeletalMeshComp = CurCharacter->GetMesh();
			if (!SkeletalMeshComp)
			{
				return false;
			}

			// 콤보 공격이 배열의 크기를 넘어서지 않게 해준다.
			const TArray<UAnimMontage*>& CurNormalAttacks = WeaponActors[Index]->GetNormalAttacks();
			if (CurNormalAttacks.Num() <= ComboIndex)
			{
				ComboIndex %= CurNormalAttacks.Num();
			}

			// 현재 재생해야하는 몽타주를 찾는다.
			UAnimMontage* CurAttackMontage = CurNormalAttacks[ComboIndex];
			UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();

			if (CurAttackMontage && AnimInstance)
			{
				// 몽타주를 재생시켜준다.
				// 입력된 버퍼 값을 초기화하고, 다음 공격을 위해 콤보 인덱스를 증가시킨다.

				float AttackSpeed = CurCharacter->GetAttackSpeed();

				AnimInstance->Montage_Play(CurAttackMontage, AttackSpeed);

				bComboInputBuffered = false;
				ComboIndex += 1;

				if (CurNormalAttacks.Num() >= ComboIndex)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void URSPlayerWeaponComponent::ResetCombo()
{
	// 공격 애니메이션이 재생된 후 더이상 입력이 없을 때 호출된다.
	// 공격에 대해 모두 기본값으로 설정
	ComboIndex = 0;
	bComboInputBuffered = false;
	bIsAttack = false;
}

void URSPlayerWeaponComponent::EquipWeaponToSlot(ARSBaseWeapon* NewWeaponActor)
{
	// 슬롯의 크기가 잘못 설정된 경우
	if (WeaponActors.Num() != WeaponSlotSize)
	{
		WeaponActors.SetNum(WeaponSlotSize);
	}

	// 슬롯에 포함할 액터를 숨김처리
	NewWeaponActor->SetActorHiddenInGame(true);

	// 캐릭터의 손에 무기를 부착한다.
	ACharacter* CurCharacter = GetOwner<ACharacter>();
	if (CurCharacter)
	{
		USkeletalMeshComponent* SkeletalMeshComp = CurCharacter->GetMesh();
		if (SkeletalMeshComp)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			NewWeaponActor->AttachToComponent(SkeletalMeshComp, AttachmentRules, FName("Weapon_Hand_r"));
		}
	}

	// 현재 비어있는 무기 슬롯이 있을 경우 비어있는 슬롯에 우선적으로 채운다.
	if (WeaponActors[0] == nullptr)
	{
		WeaponActors[0] = NewWeaponActor;

		// UI 갱신되도록 이벤트 디스패처 호출
		FName NewWeaponKey = NewWeaponActor->GetDataTableKey();

		ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(CurCharacter->GetController());
		if (PC)
		{
			PC->OnWeaponSlotChange.Broadcast(1, NewWeaponKey);
		}

		RS_LOG("0 Slot Add WeaponInfo");
	}
	else if (WeaponActors[1] == nullptr)
	{
		WeaponActors[1] = NewWeaponActor;

		// UI 갱신되도록 이벤트 디스패처 호출
		FName NewWeaponKey = NewWeaponActor->GetDataTableKey();

		ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(CurCharacter->GetController());
		if (PC)
		{
			PC->OnWeaponSlotChange.Broadcast(2, NewWeaponKey);
		}

		RS_LOG("1 Slot Add WeaponInfo");
	}
	else
	{
		// 기존에 장착하던 무기를 버리는 로직
		// 만약, 비어있는 무기 슬롯이 없는 경우 들고 있는 무기와 교체한다.
		
		int8 Index = static_cast<int8>(WeaponSlot) - 1;

		// 만약 무기를 장착중이지 않은 경우 첫 슬롯의 무기와 교체한다.
		if (EWeaponSlot::NONE == WeaponSlot)
		{
			Index = static_cast<int8>(EWeaponSlot::FirstWeaponSlot) - 1;
			WeaponSlot = EWeaponSlot::FirstWeaponSlot;
		}

		// 기존 무기를 버린다.
		DropWeaponToSlot(WeaponSlot);

		// 새로운 무기를 장착
		WeaponActors[Index] = NewWeaponActor;

		EWeaponSlot TempWeaponSlot = WeaponSlot;
		WeaponSlot = EWeaponSlot::NONE;
		EquipWeaponToCharacter(TempWeaponSlot);

		// UI 갱신되도록 이벤트 디스패처 호출
		FName NewWeaponKey = NewWeaponActor->GetDataTableKey();

		ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(CurCharacter->GetController());
		if (PC)
		{
			PC->OnWeaponSlotChange.Broadcast(static_cast<int8>(WeaponSlot), NewWeaponKey);
		}
	}
}

void URSPlayerWeaponComponent::DropWeaponToSlot(EWeaponSlot TargetWeaponSlot)
{
	// 잘못된 값이 들어왔는지 확인
	if (EWeaponSlot::NONE == TargetWeaponSlot)
	{
		return;
	}

	// 버리려는 슬롯이 비어있는 경우 취소
	int8 TargetIndex = static_cast<int8>(TargetWeaponSlot) - 1;
	if (!WeaponActors.IsValidIndex(TargetIndex) || !WeaponActors[TargetIndex])
	{
		return;
	}

	ACharacter* CurCharacter = GetOwner<ACharacter>();
	if (!CurCharacter)
	{
		return;
	}

	// 땅에 버려질 액터 생성
	ARSDungeonGroundWeapon* GroundWeapon = GetWorld()->SpawnActor<ARSDungeonGroundWeapon>(ARSDungeonGroundWeapon::StaticClass(), CurCharacter->GetActorTransform());

	// 땅에 버려질 액터에 세팅할 값을 데이터 테이블에 가져와 세팅한다.
	FName WeaponKey = WeaponActors[TargetIndex]->GetDataTableKey();

	FItemInfoData* ItemInfoData = CurCharacter->GetGameInstance()->GetSubsystem<URSDataSubsystem>()->WeaponInfo->FindRow<FItemInfoData>(WeaponKey, TEXT("Get WeaponData"));
	FDungeonWeaponData* WeaponData = GetWorld()->GetGameInstance()->GetSubsystem<URSDataSubsystem>()->WeaponDetail->FindRow<FDungeonWeaponData>(WeaponKey, TEXT("Get WeaponData"));
	if (ItemInfoData && WeaponData)
	{
		FText ItemName = ItemInfoData->ItemName;
		UStaticMesh* ItemStaticMesh = ItemInfoData->ItemStaticMesh;
		TSubclassOf<ARSDungeonItemBase> WeaponClass = WeaponData->WeaponClass;

		if (GroundWeapon && ItemStaticMesh && WeaponClass)
		{
			GroundWeapon->InitGroundItemInfo(ItemName, false, WeaponKey, ItemStaticMesh);
			GroundWeapon->SetWeaponClass(WeaponClass);
			GroundWeapon->RandImpulse();
		}
	}

	// 장착 중이었을 경우 무기 장착 해제
	if (TargetWeaponSlot == WeaponSlot)
	{
		UnEquipWeaponToCharacter();
	}

	// 기존에 소유하던 무기 제거
	WeaponActors[TargetIndex]->Destroy();
	WeaponActors[TargetIndex] = nullptr;

	// UI 갱신되도록 이벤트 디스패처 호출
	ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(CurCharacter->GetController());
	if (PC)
	{
		PC->OnWeaponSlotChange.Broadcast(static_cast<int8>(TargetWeaponSlot), FName(""));
	}
}

void URSPlayerWeaponComponent::EquipWeaponToCharacter(EWeaponSlot TargetWeaponSlot)
{
	// 잘못된 값이 들어왔는지 확인
	if (EWeaponSlot::NONE == TargetWeaponSlot)
	{
		return;
	}

	// 바꾸려는 슬롯이 비어있는 경우 취소
	int8 TargetIndex = static_cast<int8>(TargetWeaponSlot) - 1;
	if (!WeaponActors.IsValidIndex(TargetIndex) || !WeaponActors[TargetIndex])
	{
		return;
	}
	
	// 바꾸려는 슬롯이 현재 슬롯인 경우 취소
	int8 CurrentIndex = static_cast<int8>(WeaponSlot) - 1;
	if (CurrentIndex == TargetIndex)
	{
		return;
	}

	// 기존에 착용하고 있던 무기가 유효한 경우
	if (WeaponActors.IsValidIndex(CurrentIndex) && WeaponActors[CurrentIndex])
	{
		// 장착 중인 무기를 제거한다.
		UnEquipWeaponToCharacter();
	}

	// 새로 착용할 무기가 유효한 경우
	if (WeaponActors.IsValidIndex(TargetIndex) && WeaponActors[TargetIndex])
	{
		ARSBaseWeapon* TargetEquipWeapon = WeaponActors[TargetIndex];
		if (TargetEquipWeapon)
		{
			// 새로 착용할 무기의 숨김 처리를 끈다.
			TargetEquipWeapon->SetActorHiddenInGame(false);

			// 무기의 애님 레이어를 적용한다.
			TSubclassOf<UAnimInstance> TargetAnimInstance = TargetEquipWeapon->GetWeaponAnimInstnace();

			ACharacter* CurCharacter = GetOwner<ACharacter>();
			if (CurCharacter)
			{
				USkeletalMeshComponent* SkeletalMeshComp = CurCharacter->GetMesh();
				if (SkeletalMeshComp)
				{
					SkeletalMeshComp->LinkAnimClassLayers(TargetAnimInstance);
				}
			}

			// 현재 슬롯을 변경한다.
			WeaponSlot = TargetWeaponSlot;
		}
	}
}

void URSPlayerWeaponComponent::UnEquipWeaponToCharacter()
{
	// 잘못된 값이 들어왔는지 확인
	if (EWeaponSlot::NONE == WeaponSlot)
	{
		return;
	}

	int8 CurrentIndex = static_cast<int8>(WeaponSlot) - 1;

	// 현재 착용 중인 무기가 있는 경우
	ARSBaseWeapon* CurEquipWeapon = WeaponActors[CurrentIndex];
	if (CurEquipWeapon)
	{
		// 숨김 처리
		CurEquipWeapon->SetActorHiddenInGame(true);

		// 무기의 애님 레이어를 해제한다.
		TSubclassOf<UAnimInstance> CurAnimInstance = CurEquipWeapon->GetWeaponAnimInstnace();

		ACharacter* CurCharacter = GetOwner<ACharacter>();
		if (CurCharacter)
		{
			USkeletalMeshComponent* SkeletalMeshComp = CurCharacter->GetMesh();
			if (SkeletalMeshComp)
			{
				SkeletalMeshComp->UnlinkAnimClassLayers(CurAnimInstance);
			}
		}

		// 현재 슬롯을 변경한다.
		WeaponSlot = EWeaponSlot::NONE;
	}
}

void URSPlayerWeaponComponent::ResetDamagedActors()
{
	// 해당 무기로 피해를 입은 액터들의 목록을 초기화한다.
	DamagedActors.Empty();
}

void URSPlayerWeaponComponent::PerformBoxSweepAttack()
{
	// 무기 슬롯의 인덱스 값
	int8 Index = static_cast<int8>(WeaponSlot) - 1;

	ARSDunPlayerCharacter* OwnerCharacter = GetOwner<ARSDunPlayerCharacter>();

	if (OwnerCharacter)
	{
		AController* OwnerController = OwnerCharacter->GetInstigatorController();

		if (WeaponActors.IsValidIndex(Index))
		{
			ARSBaseWeapon* CurWeapon = WeaponActors[Index];

			UBoxComponent* WeaponBoxComp = CurWeapon->GetBoxComp();
			if (WeaponBoxComp)
			{
				FVector BoxLocation = WeaponBoxComp->GetComponentLocation();
				FVector BoxExtent = WeaponBoxComp->GetUnscaledBoxExtent() * WeaponBoxComp->GetComponentScale();
				FQuat BoxRotation = WeaponBoxComp->GetComponentQuat();

				FVector Start = BoxLocation;
				FVector End = BoxLocation + BoxRotation.RotateVector(FVector(BoxExtent.X * 2.f, 0.f, 0.f));

				FCollisionShape BoxShape = FCollisionShape::MakeBox(BoxExtent);
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(GetOwner());

				TArray<FHitResult> HitResults;
				bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, BoxRotation, ECC_GameTraceChannel8, BoxShape, Params);

				// 시작 위치 박스 디버그
				RS_DRAW_DEBUG_BOX(GetWorld(), Start, BoxExtent, BoxRotation, FColor::Green, false, 2.0f, 0, 1.0f);

				// 끝 위치 박스 디버그
				RS_DRAW_DEBUG_BOX(GetWorld(), End, BoxExtent, BoxRotation, FColor::Blue, false, 2.0f, 0, 1.0f);

				if (bHit)
				{
					for (const FHitResult& Hit : HitResults)
					{
						AActor* HitActor = Hit.GetActor();
						if (HitActor && HitActor != GetOwner() && !DamagedActors.Contains(HitActor))
						{
							// 총 데미지를 구합니다.
							float TotalDamage = 0.f;

							float WeaponDamage = WeaponActors[Index]->GetWeaponDamage();
							float AttackPower = OwnerCharacter->GetAttackPower();
							TotalDamage = WeaponDamage + AttackPower;

							// 피격 방향 계산
							FVector HitDirection = HitActor->GetActorLocation() - Hit.ImpactPoint;
							HitDirection.Normalize();

							UGameplayStatics::ApplyPointDamage(HitActor, TotalDamage, HitDirection, Hit, OwnerController, WeaponActors[Index], UDamageType::StaticClass());

							// 무기 데미지를 중복으로 주는 경우를 방지하기 위한 배열에 값 추가
							DamagedActors.Add(HitActor);

							// 데미지가 발생한 포인트에 대해 디버그
							RS_DRAW_DEBUG_POINT(GetWorld(), Hit.ImpactPoint, 10.f, FColor::Red, false, 2.0f, 0);
						}
					}
				}
			}
		}
	}
}

void URSPlayerWeaponComponent::SaveRequested()
{
	// SaveGame 오브젝트 생성
	URSDungeonWeaponSaveGame* WeaponSaveGame = Cast<URSDungeonWeaponSaveGame>(UGameplayStatics::CreateSaveGameObject(URSDungeonWeaponSaveGame::StaticClass()));

	WeaponSaveGame->WeaponActors.SetNum(WeaponSlotSize);
	// 무기 정보 저장
	for (int i = 0; i < WeaponActors.Num(); ++i)
	{
		ARSBaseWeapon* Weapon = WeaponActors[i];
		if (Weapon)
		{
			WeaponSaveGame->WeaponActors[i] = Weapon->GetDataTableKey();
		}
	}

	// 장착 중인 무기 슬롯 저장
	int8 CurWeaponSlot = static_cast<int8>(WeaponSlot);
	WeaponSaveGame->WeaponSlot = CurWeaponSlot;
	
	// 저장
	ARSDunPlayerCharacter* OwnerCharacter = GetOwner<ARSDunPlayerCharacter>();
	if (!OwnerCharacter)
	{
		return;
	}
	UGameInstance* CurGameInstance = OwnerCharacter->GetGameInstance();
	if (!CurGameInstance)
	{
		return;
	}

	URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
	if (!SaveGameSubsystem)
	{
		return;
	}

	UGameplayStatics::SaveGameToSlot(WeaponSaveGame, SaveGameSubsystem->WeaponSaveSlotName, 0);
}

void URSPlayerWeaponComponent::LoadRequested()
{
	// 세이브가 있는지 확인 후 로드
	ARSDunPlayerCharacter* OwnerCharacter = GetOwner<ARSDunPlayerCharacter>();
	if (!OwnerCharacter)
	{
		return;
	}
	UGameInstance* CurGameInstance = OwnerCharacter->GetGameInstance();
	if (!CurGameInstance)
	{
		return;
	}

	URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
	if (!SaveGameSubsystem)
	{
		return;
	}

	URSDungeonWeaponSaveGame* WeaponLoadGame = Cast<URSDungeonWeaponSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSubsystem->WeaponSaveSlotName, 0));
	if (!WeaponLoadGame)
	{
		return;
	}

	URSDataSubsystem* DataSubsystem = CurGameInstance->GetSubsystem<URSDataSubsystem>();
	if (!DataSubsystem)
	{
		return;
	}

	UDataTable* WeaponDataTable = DataSubsystem->WeaponInfo;
	UDataTable* WeaponDetailDataTable = DataSubsystem->WeaponDetail;
	if (!WeaponDataTable)
	{
		return;
	}

	// 저장된 무기 정보를 가져온다.
	TArray<FName> LoadWeaponName = WeaponLoadGame->WeaponActors;

	// 저장된 무기 정보를 기준으로 장착할 무기 액터를 생성하고 슬롯에 저장한다.
	for (int i = 0; i < LoadWeaponName.Num(); ++i)
	{
		if (LoadWeaponName[i].IsNone())
		{
			continue;
		}

		FName CurWeaponName = WeaponLoadGame->WeaponActors[i];

		FItemInfoData* Data = WeaponDataTable->FindRow<FItemInfoData>(CurWeaponName, TEXT("Get WeaponData"));
		FDungeonWeaponData* WeaponData = WeaponDetailDataTable->FindRow<FDungeonWeaponData>(CurWeaponName, TEXT("Get WeaponData"));

		if (Data && WeaponData && WeaponData->WeaponClass)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = OwnerCharacter;
			SpawnParameters.Instigator = OwnerCharacter;

			ARSBaseWeapon* SpawnWeapon = GetWorld()->SpawnActor<ARSBaseWeapon>(WeaponData->WeaponClass, SpawnParameters);

			if (SpawnWeapon)
			{
				SpawnWeapon->SetDataTableKey(CurWeaponName);
				EquipWeaponToSlot(SpawnWeapon);
			}
		}
	}

	// 장착 해야하는 슬롯을 가져오고 무기를 장착한다.
	EWeaponSlot TargetWeaponSlot = static_cast<EWeaponSlot>(WeaponLoadGame->WeaponSlot);
	EquipWeaponToCharacter(TargetWeaponSlot);
}
