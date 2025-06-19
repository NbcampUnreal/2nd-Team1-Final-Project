// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "WeaponAttackData.h"

ARSBaseWeapon::ARSBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	MeshComp->SetupAttachment(SceneComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComp->SetupAttachment(SceneComp);
	BoxComp->SetCollisionProfileName("PlayerAttackHitBox");
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComp->SetGenerateOverlapEvents(false);

	SetActorEnableCollision(false);
}

void ARSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

UBoxComponent* ARSBaseWeapon::GetBoxComp() const
{
	return BoxComp;
}

TSubclassOf<UAnimInstance> ARSBaseWeapon::GetWeaponAnimInstnace() const
{
	return WeaponAnimInstnace;
}

UAnimMontage* ARSBaseWeapon::GetNormalAttackMontage(int32 Index) const
{
	if (NormalAttackDatas.Num() > Index)
	{
		return NormalAttackDatas[Index].AttackMontage;
	}

	return nullptr;
}

const TArray<UAnimMontage*> ARSBaseWeapon::GetNormalAttackMontages() const
{
	TArray<UAnimMontage*> NormalAttackMontages;

	for (const auto& e : NormalAttackDatas)
	{
		NormalAttackMontages.Add(e.AttackMontage);
	}

	return NormalAttackMontages;
}

UAnimMontage* ARSBaseWeapon::GetStrongAttackMontage(int32 Index) const
{
	if (StrongAttackDatas.Num() > Index)
	{
		return StrongAttackDatas[Index].AttackMontage;
	}

	return nullptr;
}

const TArray<UAnimMontage*> ARSBaseWeapon::GetStrongAttackMontages() const
{
	TArray<UAnimMontage*> StrongAttackMontages;

	for (const auto& e : StrongAttackDatas)
	{
		StrongAttackMontages.Add(e.AttackMontage);
	}

	return StrongAttackMontages;
}

void ARSBaseWeapon::StartOverlap()
{
	// 트레이스 채널을 사용하기 때문에 더이상 사용하지 않는 기능

	// 콜리전과 오버랩 이벤트를 켠다.
	SetActorEnableCollision(true);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetGenerateOverlapEvents(true);
}

void ARSBaseWeapon::EndOverlap()
{
	// 트레이스 채널을 사용하기 때문에 더이상 사용하지 않는 기능

	// 콜리전과 오버랩 이벤트를 끈다.
	SetActorEnableCollision(false);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComp->SetGenerateOverlapEvents(false);
}

float ARSBaseWeapon::GetNormalAttackMontageDamage(int32 MontageIndex) const
{
	if (NormalAttackDatas.IsValidIndex(MontageIndex))
	{
		return NormalAttackDatas[MontageIndex].WeaponDamage;
	}

	return 0.f;
}

float ARSBaseWeapon::GetStrongAttackMontageDamage(int32 MontageIndex) const
{
	if (StrongAttackDatas.IsValidIndex(MontageIndex))
	{
		return StrongAttackDatas[MontageIndex].WeaponDamage;
	}

	return 0.0f;
}

void ARSBaseWeapon::StartTrail()
{
	if (TrailEffect && BoxComp)
	{
		TrailNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailEffect, SceneComp, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, true);
		
		TrailNiagaraComp->Activate();

		// Trail 나이아가라 크기 및 위치 조절
		// 박스 컴포넌트의 월드 기준 크기의 절반
		FVector BoxExtent = BoxComp->GetScaledBoxExtent();

		// 4배 크기로 설정
		// 2배할 경우 박스의 월드 기준 크기
		// 한번 더 2배하여 Trail에 맞는 크기 설정
		FVector BoxLength = BoxExtent * 4;
		TrailNiagaraComp->SetFloatParameter(TEXT("User.EffectWidth"), BoxLength.Z);

		// 박스 컴포넌트의 월드 기준 위치
		FVector BoxWorldLocation = BoxComp->GetComponentLocation();

		// 박스 컴포넌트의 최하단 위치
		FVector Position = { BoxWorldLocation.X, BoxWorldLocation.Y, BoxWorldLocation.Z - BoxExtent.Z };

		// TrailNiagaraComp의 상대 위치로 변환
		FVector RelativePosition = TrailNiagaraComp->GetComponentTransform().InverseTransformPosition(Position);

		TrailNiagaraComp->SetVectorParameter(TEXT("User.EffectPosition"), RelativePosition);
	}
}

void ARSBaseWeapon::EndTrail()
{
	if (TrailNiagaraComp && TrailNiagaraComp->IsValidLowLevel())
	{
		// 기존 Trail 이펙트는 비활성화한다.
		TrailNiagaraComp->Deactivate();

		// 나이아가라가 Loop되는 경우 자동적으로 제거되지 않는다.
		// 그렇기 때문에 명시적으로 3초 후 제거한다.
		TWeakObjectPtr<UNiagaraComponent> DeactivateNiagara = TrailNiagaraComp;

		FTimerHandle TrailTimer;

		GetWorld()->GetTimerManager().SetTimer(TrailTimer, [DeactivateNiagara]()
			{
				if (DeactivateNiagara.IsValid())
				{
					DeactivateNiagara->DestroyComponent();
				}
			}, 2.f, false);

		TrailNiagaraComp = nullptr;
	}
}

void ARSBaseWeapon::SpawnHitImpactEffect(FVector TargetLocation)
{
	if (HitImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitImpactEffect, TargetLocation);
	}
}

FName ARSBaseWeapon::GetDataTableKey() const
{
	return DataTableKey;
}

void ARSBaseWeapon::SetDataTableKey(FName NewDataTableKey)
{
	DataTableKey = NewDataTableKey;

	// 데이터 테이블 키 값으로 현재 액터 초기화
	Initialization();
}

void ARSBaseWeapon::Initialization()
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

	UDataTable* WeaponDetailDataTable = DataSubsystem->WeaponDetail;
	if (!WeaponDetailDataTable)
	{
		return;
	}

	FDungeonWeaponData* WeaponData = WeaponDetailDataTable->FindRow<FDungeonWeaponData>(DataTableKey, TEXT("Get WeaponData"));

	if (WeaponData)
	{
		NormalAttackDatas = WeaponData->NormalAttackData;
		StrongAttackDatas = WeaponData->StrongAttackData;

		TrailEffect = WeaponData->WeaponTrail;

		HitImpactEffect = WeaponData->HitImpactEffect;
	}
}
