// Fill out your copyright notice in the Description page of Project Settings.


#include "RSPlayerWeaponComponent.h"
#include "RogShop/UtilDefine.h"
#include "RSBaseWeapon.h"
#include "RSDunPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "RogShop/GameInstanceSubsystem/RSDataSubsystem.h"
#include "DungeonItemData.h"
#include "RSInteractableWeapon.h"

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

	// ...
	
}


// Called every frame
void URSPlayerWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URSPlayerWeaponComponent::HandleNormalAttackInput()
{
    if (bIsAttack)
    {
        bComboInputBuffered = true;
    }
    else
    {

		uint8 Index = static_cast<uint8>(WeaponSlot);
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
		uint8 Index = static_cast<uint8>(WeaponSlot);
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

		RS_LOG("0 Slot Add Weapon")
	}
	else if (WeaponActors[1] == nullptr)
	{
		WeaponActors[1] = NewWeaponActor;

		RS_LOG("1 Slot Add Weapon")
	}
	else
	{
		// ����, ����ִ� ���� ������ ���� ��� ��� �ִ� ����� ��ü�Ѵ�.
		// TODO : ���� ����� �� �ٴڿ� ���������� �����ؾ��Ѵ�.
		// ������ �����ϴ� ���⸦ ������ ����
		uint8 Index = static_cast<uint8>(WeaponSlot);

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

			// ������ �����ϴ� ���� ����
			WeaponActors[Index]->Destroy();
			WeaponActors[Index] = NewWeaponActor;

			// ���ο� ���⸦ ����
			EquipWeaponToCharacter(WeaponSlot);
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
	
	// TODO : WeaponSlot == TargetWeaponSlot�� ���� ���� ��� ���� ������ ������ ���� ó���� ����, �浹�� �ѱ⸸ �Ѵ�.

	// ���� ���� ���� ���Ⱑ �ִ� ��� ���� ó�� �� �浹�� ����.
	// ���� ������ ������ ���� ó���� ����, �浹�� �Ҵ�.
	uint8 Index = static_cast<uint8>(WeaponSlot);
	if (WeaponActors.IsValidIndex(Index))
	{
		ARSBaseWeapon* CurEquipWeapon = WeaponActors[Index];
		if (CurEquipWeapon)
		{
			CurEquipWeapon->SetActorHiddenInGame(true);
			CurEquipWeapon->SetActorEnableCollision(false);

			// ������ �̺�Ʈ ���ε� ����
			WeaponActors[Index]->GetBoxComp()->OnComponentBeginOverlap.RemoveDynamic(this, &URSPlayerWeaponComponent::OnBeginOverlap);
		}
	}

	uint8 TargetIndex = static_cast<uint8>(TargetWeaponSlot);
	if (WeaponActors.IsValidIndex(TargetIndex))
	{
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
		}
	}
	// ���� ������ �����Ѵ�.
	WeaponSlot = TargetWeaponSlot;
}

void URSPlayerWeaponComponent::StartAttackOverlap()
{
	// �ݸ����� �Ҵ�.
	uint8 Index = static_cast<uint8>(WeaponSlot);
	if (WeaponActors.IsValidIndex(Index))
	{
		WeaponActors[Index]->StartOverlap();
	}
}

void URSPlayerWeaponComponent::EndAttackOverlap()
{
	// �ݸ����� ����.
	uint8 Index = static_cast<uint8>(WeaponSlot);
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
		uint8 Index = static_cast<uint8>(WeaponSlot);
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