// Fill out your copyright notice in the Description page of Project Settings.


#include "RSPlayerWeaponComponent.h"
#include "RogShop/UtilDefine.h"
#include "RSBaseWeapon.h"
#include "RSDunPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "RSDataSubsystem.h"
#include "DungeonItemData.h"
#include "RSInteractableWeapon.h"
#include "RSDunPlayerController.h"
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
	if (OwnerCharacter)
	{
		OwnerCharacter->OnSaveRequested.AddDynamic(this, &URSPlayerWeaponComponent::SaveRequested);
	}
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

			// �޺� ������ �迭�� ũ�⸦ �Ѿ�� �ʰ� ���ش�.
			const TArray<UAnimMontage*>& CurNormalAttacks = WeaponActors[Index]->GetNormalAttacks();
			if (CurNormalAttacks.Num() <= ComboIndex)
			{
				ComboIndex %= CurNormalAttacks.Num();
			}

			// ���� ����ؾ��ϴ� ��Ÿ�ָ� ã�´�.
			UAnimMontage* CurAttackMontage = CurNormalAttacks[ComboIndex];
			UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();

			if (CurAttackMontage && AnimInstance)
			{
				// ��Ÿ�ָ� ��������ش�.
				// �Էµ� ���� ���� �ʱ�ȭ�ϰ�, ���� ������ ���� �޺� �ε����� ������Ų��.

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
	// ���� �ִϸ��̼��� ����� �� ���̻� �Է��� ���� �� ȣ��ȴ�.
	// ���ݿ� ���� ��� �⺻������ ����
	ComboIndex = 0;
	bComboInputBuffered = false;
	bIsAttack = false;
}

void URSPlayerWeaponComponent::EquipWeaponToSlot(ARSBaseWeapon* NewWeaponActor)
{
	// ������ ũ�Ⱑ �߸� ������ ���
	if (WeaponActors.Num() != WeaponSlotSize)
	{
		WeaponActors.SetNum(WeaponSlotSize);
	}

	// ���Կ� ������ ���͸� ����ó�� �� �浹�� ����.
	NewWeaponActor->SetActorHiddenInGame(true);
	NewWeaponActor->SetActorEnableCollision(false);

	// ĳ������ �տ� ���⸦ �����Ѵ�.
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

	// ���� ����ִ� ���� ������ ���� ��� ����ִ� ���Կ� �켱������ ä���.
	if (WeaponActors[0] == nullptr)
	{
		WeaponActors[0] = NewWeaponActor;

		// UI ���ŵǵ��� �̺�Ʈ ����ó ȣ��
		FName NewWeaponKey = NewWeaponActor->GetDataTableKey();

		ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(CurCharacter->GetController());
		if (PC)
		{
			PC->OnWeaponSlotChange.Broadcast(1, NewWeaponKey);
		}

		RS_LOG("0 Slot Add Weapon")
	}
	else if (WeaponActors[1] == nullptr)
	{
		WeaponActors[1] = NewWeaponActor;

		// UI ���ŵǵ��� �̺�Ʈ ����ó ȣ��
		FName NewWeaponKey = NewWeaponActor->GetDataTableKey();

		ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(CurCharacter->GetController());
		if (PC)
		{
			PC->OnWeaponSlotChange.Broadcast(2, NewWeaponKey);
		}

		RS_LOG("1 Slot Add Weapon")
	}
	else
	{
		// ������ �����ϴ� ���⸦ ������ ����
		// ����, ����ִ� ���� ������ ���� ��� ��� �ִ� ����� ��ü�Ѵ�.
		
		int8 Index = static_cast<int8>(WeaponSlot) - 1;

		// ���� ���⸦ ���������� ���� ��� ù ������ ����� ��ü�Ѵ�.
		if (EWeaponSlot::NONE == WeaponSlot)
		{
			Index = static_cast<int8>(EWeaponSlot::FirstWeaponSlot) - 1;
		}

		ARSInteractableWeapon* InteractableWeapon = GetWorld()->SpawnActor<ARSInteractableWeapon>(ARSInteractableWeapon::StaticClass(), CurCharacter->GetActorTransform());

		FName WeaponKey = WeaponActors[Index]->GetDataTableKey();

		FDungeonItemData* Data = CurCharacter->GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Weapon->FindRow<FDungeonItemData>(WeaponKey, TEXT("Get WeaponData"));

		if (Data)
		{
			UStaticMesh* ItemStaticMesh = Data->ItemStaticMesh;
			TSubclassOf<ARSDungeonItemBase> ItemClass = Data->ItemClass;

			if (InteractableWeapon && ItemStaticMesh && ItemClass)
			{
				InteractableWeapon->InitInteractableWeapon(WeaponKey, ItemStaticMesh, ItemClass);
			}
		}

		// ������ �����ϴ� ���� ����
		WeaponActors[Index]->Destroy();
		WeaponActors[Index] = NewWeaponActor;

		// ���ο� ���⸦ ����
		EWeaponSlot TempWeaponSlot = WeaponSlot;
		WeaponSlot = EWeaponSlot::NONE;
		EquipWeaponToCharacter(TempWeaponSlot);

		// UI ���ŵǵ��� �̺�Ʈ ����ó ȣ��
		FName NewWeaponKey = NewWeaponActor->GetDataTableKey();

		ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(CurCharacter->GetController());
		if (PC)
		{
			PC->OnWeaponSlotChange.Broadcast(static_cast<int8>(WeaponSlot), NewWeaponKey);
		}
	}
}

void URSPlayerWeaponComponent::EquipWeaponToCharacter(EWeaponSlot TargetWeaponSlot)
{
	// �߸��� ���� ���Դ��� Ȯ��
	if (EWeaponSlot::NONE == TargetWeaponSlot)
	{
		return;
	}

	// �ٲٷ��� ������ ����ִ� ��� ���
	int8 TargetIndex = static_cast<int8>(TargetWeaponSlot) - 1;
	if (!WeaponActors.IsValidIndex(TargetIndex) || !WeaponActors[TargetIndex])
	{
		return;
	}
	
	// �ٲٷ��� ������ ���� ������ ��� ���
	int8 CurrentIndex = static_cast<int8>(WeaponSlot) - 1;
	if (CurrentIndex == TargetIndex)
	{
		return;
	}

	// ������ �����ϰ� �ִ� ���Ⱑ ��ȿ�� ���
	if (WeaponActors.IsValidIndex(CurrentIndex) && WeaponActors[CurrentIndex])
	{
		// ���� ���� ���� ���Ⱑ �ִ� ��� ���� ó�� �� �浹�� ����.
		ARSBaseWeapon* CurEquipWeapon = WeaponActors[CurrentIndex];
		if (CurEquipWeapon)
		{
			CurEquipWeapon->SetActorHiddenInGame(true);
			CurEquipWeapon->SetActorEnableCollision(false);

			// ������ �̺�Ʈ ���ε� ����
			WeaponActors[CurrentIndex]->GetBoxComp()->OnComponentBeginOverlap.RemoveDynamic(this, &URSPlayerWeaponComponent::OnBeginOverlap);

			// ������ �ִ� ���̾ �����Ѵ�.
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
		}
	}

	// ���� ������ ���Ⱑ ��ȿ�� ���
	if (WeaponActors.IsValidIndex(TargetIndex) && WeaponActors[TargetIndex])
	{
		// ���� ������ ������ ���� ó���� ����, �浹�� �Ҵ�.
		ARSBaseWeapon* TargetEquipWeapon = WeaponActors[TargetIndex];
		if (TargetEquipWeapon)
		{
			TargetEquipWeapon->SetActorHiddenInGame(false);
			TargetEquipWeapon->SetActorEnableCollision(true);
			
			// ������ �̺�Ʈ ���ε�
			UBoxComponent* CurWeaponBoxComp = TargetEquipWeapon->GetBoxComp();
			if (CurWeaponBoxComp)
			{
				CurWeaponBoxComp->OnComponentBeginOverlap.AddDynamic(this, &URSPlayerWeaponComponent::OnBeginOverlap);
			}

			// ������ �ִ� ���̾ �����Ѵ�.
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

			// ���� ������ �����Ѵ�.
			WeaponSlot = TargetWeaponSlot;
		}
	}
}

void URSPlayerWeaponComponent::StartAttackOverlap()
{
	// �ݸ����� �Ҵ�.
	int8 Index = static_cast<int8>(WeaponSlot) - 1;
	if (WeaponActors.IsValidIndex(Index))
	{
		WeaponActors[Index]->StartOverlap();
	}
}

void URSPlayerWeaponComponent::EndAttackOverlap()
{
	// �ݸ����� ����.
	int8 Index = static_cast<int8>(WeaponSlot) - 1;
	if (WeaponActors.IsValidIndex(Index))
	{
		WeaponActors[Index]->EndOverlap();
	}

	// �ش� ����� ���ظ� ���� ���͵��� ����� �ʱ�ȭ�Ѵ�.
	DamagedActors.Empty();
}

void URSPlayerWeaponComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ������ �� ���Ϳ��� �������� ���Ѵ�.
	ARSDunPlayerCharacter* OwnerCharacter = GetOwner<ARSDunPlayerCharacter>();

	AController* OwnerController = nullptr;
	if (OwnerCharacter)
	{
		OwnerController = OwnerCharacter->GetInstigatorController();
	}

	if (OtherActor && OtherActor != OwnerCharacter && !DamagedActors.Contains(OtherActor))
	{
		int8 Index = static_cast<int8>(WeaponSlot) - 1;
		float TotalDamage = 0.f;
		if (WeaponActors.IsValidIndex(Index) && OwnerCharacter)
		{
			float WeaponDamage = WeaponActors[Index]->GetWeaponDamage();
			float AttackPower = OwnerCharacter->GetAttackPower();
			TotalDamage = WeaponDamage + AttackPower;
		}

		UGameplayStatics::ApplyDamage(OtherActor, TotalDamage, OwnerController, WeaponActors[Index], UDamageType::StaticClass());
		DamagedActors.Add(OtherActor);
	}
}

void URSPlayerWeaponComponent::SaveRequested()
{
	// SaveGame ������Ʈ ����
	URSDungeonWeaponSaveGame* WeaponSaveGame = Cast<URSDungeonWeaponSaveGame>(UGameplayStatics::CreateSaveGameObject(URSDungeonWeaponSaveGame::StaticClass()));

	WeaponSaveGame->WeaponActors.SetNum(WeaponSlotSize);
	// ���� ���� ����
	for (int i = 0; i < WeaponActors.Num(); ++i)
	{
		ARSBaseWeapon* Weapon = WeaponActors[i];
		if (Weapon)
		{
			WeaponSaveGame->WeaponActors[i] = Weapon->GetDataTableKey();
		}
	}

	// ���� ���� ���� ���� ����
	int8 CurWeaponSlot = static_cast<int8>(WeaponSlot);
	WeaponSaveGame->WeaponSlot = CurWeaponSlot;
	
	// ����
	UGameplayStatics::SaveGameToSlot(WeaponSaveGame, WeaponSaveSlotName, 0);
}

void URSPlayerWeaponComponent::LoadRequested()
{
	// �� üũ
	URSDungeonWeaponSaveGame* WeaponLoadGame = Cast<URSDungeonWeaponSaveGame>(UGameplayStatics::LoadGameFromSlot(WeaponSaveSlotName, 0));
	if (!WeaponLoadGame)
	{
		return;
	}


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

	URSDataSubsystem* DataSubsystem = CurGameInstance->GetSubsystem<URSDataSubsystem>();
	if (!DataSubsystem)
	{
		return;
	}

	UDataTable* WeaponDataTable = DataSubsystem->Weapon;
	if (!WeaponDataTable)
	{
		return;
	}

	// ����� ���� ������ �����´�.
	TArray<FName> LoadWeaponName = WeaponLoadGame->WeaponActors;

	// ����� ���� ������ �������� ������ ���� ���͸� �����ϰ� ���Կ� �����Ѵ�.
	for (int i = 0; i < LoadWeaponName.Num(); ++i)
	{
		if (LoadWeaponName[i].IsNone())
		{
			continue;
		}

		FName CurWeaponName = WeaponLoadGame->WeaponActors[i];

		FDungeonItemData* Data = WeaponDataTable->FindRow<FDungeonItemData>(CurWeaponName, TEXT("Get WeaponData"));

		if (Data && Data->ItemClass)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = OwnerCharacter;
			SpawnParameters.Instigator = OwnerCharacter;

			ARSBaseWeapon* SpawnWeapon = GetWorld()->SpawnActor<ARSBaseWeapon>(Data->ItemClass, SpawnParameters);

			if (SpawnWeapon)
			{
				SpawnWeapon->SetDataTableKey(CurWeaponName);
				EquipWeaponToSlot(SpawnWeapon);
			}
		}
	}

	// ���� �ؾ��ϴ� ������ �������� ���⸦ �����Ѵ�.
	EWeaponSlot TargetWeaponSlot = static_cast<EWeaponSlot>(WeaponLoadGame->WeaponSlot);
	EquipWeaponToCharacter(TargetWeaponSlot);
}
