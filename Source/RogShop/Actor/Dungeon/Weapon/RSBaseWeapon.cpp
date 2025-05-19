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

UAnimMontage* ARSBaseWeapon::GetNormalAttack(int32 Index)
{
	if (NormalAttacks.Num() > Index)
	{
		return NormalAttacks[Index];
	}

	return nullptr;
}

const TArray<UAnimMontage*>& ARSBaseWeapon::GetNormalAttacks()
{
	return NormalAttacks;
}

void ARSBaseWeapon::StartOverlap()
{
	// 콜리전을 켠다.
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ARSBaseWeapon::EndOverlap()
{
	// 콜리전을 끈다.
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float ARSBaseWeapon::GetWeaponDamage() const
{
	return WeaponDamage;
}
