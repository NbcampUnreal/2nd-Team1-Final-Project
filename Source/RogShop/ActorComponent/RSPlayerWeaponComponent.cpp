// Fill out your copyright notice in the Description page of Project Settings.


#include "RSPlayerWeaponComponent.h"
#include "RogShop/UtilDefine.h"
#include "RSBaseWeapon.h"
#include "RSDunPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values for this component's properties
URSPlayerWeaponComponent::URSPlayerWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	WeaponActors.SetNum(2);
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

			const TArray<UAnimMontage*>& CurNormalAttacks = WeaponActors[Index]->GetNormalAttacks();
			if (CurNormalAttacks.Num() <= ComboIndex)
			{
				ComboIndex %= CurNormalAttacks.Num();
			}

			UAnimMontage* CurAttackMontage = CurNormalAttacks[ComboIndex];
			UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();

			if (CurAttackMontage && AnimInstance)
			{
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
	ComboIndex = 0;
	bComboInputBuffered = false;
	bIsAttack = false;
}

void URSPlayerWeaponComponent::EquipWeaponToSlot(ARSBaseWeapon* NewWeaponActor)
{
	// 슬롯의 크기가 잘못 설정된 경우
	if (WeaponActors.Num() != 2)
	{
		WeaponActors.SetNum(2);
	}

	// 슬롯에 포함할 액터를 숨김처리 및 충돌을 끈다.
	NewWeaponActor->SetActorHiddenInGame(true);
	NewWeaponActor->SetActorEnableCollision(false);

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

		RS_LOG("0 Slot Add Weapon")
	}
	else if (WeaponActors[1] == nullptr)
	{
		WeaponActors[1] = NewWeaponActor;

		RS_LOG("1 Slot Add Weapon")
	}
	
	// 만약, 비어있는 무기 슬롯이 없는 경우 들고 있는 무기와 교체한다.
	// TODO
	// 기존 무기는 땅 바닥에 버려지도록 구현해야한다.
	uint8 Index = static_cast<uint8>(WeaponSlot);
	if (WeaponActors.Num() > Index)
	{
		WeaponActors[Index] = NewWeaponActor;
		EquipWeaponToCharacter(WeaponSlot);
	}
}

void URSPlayerWeaponComponent::EquipWeaponToCharacter(EWeaponSlot TargetWeaponSlot)
{
	// 잘못된 값이 들어왔는지 확인
	if (EWeaponSlot::NONE == TargetWeaponSlot || WeaponSlot == TargetWeaponSlot)
	{
		return;
	}

	// 현재 착용 중인 무기가 있는 경우 숨김 처리 및 충돌을 끈다.
	// 새로 착용할 무기의 숨김 처리를 끄고, 충돌을 켠다.
	uint8 Index = static_cast<uint8>(WeaponSlot);
	if (WeaponActors.IsValidIndex(Index))
	{
		ARSBaseWeapon* CurEquipWeapon = WeaponActors[Index];
		if (CurEquipWeapon)
		{
			CurEquipWeapon->SetActorHiddenInGame(true);
			CurEquipWeapon->SetActorEnableCollision(false);

			// 오버랩 이벤트 바인딩 해제
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

			// 오버랩 이벤트 바인딩
			UBoxComponent* CurWeaponBoxComp = TargetEquipWeapon->GetBoxComp();
			if (CurWeaponBoxComp)
			{
				CurWeaponBoxComp->OnComponentBeginOverlap.AddDynamic(this, &URSPlayerWeaponComponent::OnBeginOverlap);
			}
		}
	}
	// 현재 슬롯을 변경한다.
	WeaponSlot = TargetWeaponSlot;
}

void URSPlayerWeaponComponent::StartAttackOverlap()
{
	// 콜리전을 켠다.
	uint8 Index = static_cast<uint8>(WeaponSlot);
	if (WeaponActors.IsValidIndex(Index))
	{
		WeaponActors[Index]->StartOverlap();
	}
}

void URSPlayerWeaponComponent::EndAttackOverlap()
{
	// 콜리전을 끈다.
	uint8 Index = static_cast<uint8>(WeaponSlot);
	if (WeaponActors.IsValidIndex(Index))
	{
		WeaponActors[Index]->EndOverlap();
	}

	// 해당 무기로 피해를 입은 액터들의 목록을 초기화한다.
	DamagedActors.Empty();
}

void URSPlayerWeaponComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 오버랩 된 액터에게 데미지를 가한다.
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