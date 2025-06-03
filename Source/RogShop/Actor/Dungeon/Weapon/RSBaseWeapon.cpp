// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"

// Sets default values
ARSBaseWeapon::ARSBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

	WeaponDamage = 0.f;
}

// Called when the game starts or when spawned
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

UAnimMontage* ARSBaseWeapon::GetNormalAttack(int32 Index) const
{
	if (NormalAttacks.Num() > Index)
	{
		return NormalAttacks[Index];
	}

	return nullptr;
}

const TArray<UAnimMontage*>& ARSBaseWeapon::GetNormalAttacks() const
{
	return NormalAttacks;
}

void ARSBaseWeapon::StartOverlap()
{
	// 콜리전과 오버랩 이벤트를 켠다.
	SetActorEnableCollision(true);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetGenerateOverlapEvents(true);
}

void ARSBaseWeapon::EndOverlap()
{
	// 콜리전과 오버랩 이벤트를 끈다.
	SetActorEnableCollision(false);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComp->SetGenerateOverlapEvents(false);
}

float ARSBaseWeapon::GetWeaponDamage() const
{
	return WeaponDamage;
}

FName ARSBaseWeapon::GetDataTableKey() const
{
	return DataTableKey;
}

void ARSBaseWeapon::SetDataTableKey(FName NewDataTableKey)
{
	DataTableKey = NewDataTableKey;
}
