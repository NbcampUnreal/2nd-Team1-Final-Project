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

	TrailNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	TrailNiagaraComp->SetupAttachment(SceneComp);
	TrailNiagaraComp->SetAutoActivate(false);

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
	if (TrailNiagaraComp)
	{
		TrailNiagaraComp->Activate();
	}
}

void ARSBaseWeapon::EndTrail()
{
	if (TrailNiagaraComp)
	{
		TrailNiagaraComp->Deactivate();
	}
}

void ARSBaseWeapon::SpawnHitImpactEffect(FVector TargetLocation)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitImpactEffect, TargetLocation);
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

		TrailNiagaraComp->SetAsset(WeaponData->WeaponTrail);

		HitImpactEffect = WeaponData->HitImpactEffect;

		//TODO : 서승우님이 나이아가라 크기 조절에 대해 알아봐주시고 아래 코드 사용하기
		//FVector BoxExtent = BoxComp->GetScaledBoxExtent();
		//FVector BoxLength = BoxExtent * 2;
		//TrailNiagaraComp->SetVectorParameter(TEXT("MeshBounds"), BoxLength);
	}
}
