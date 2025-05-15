// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseWeapon.h"

// Sets default values
ARSBaseWeapon::ARSBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	MeshComp->SetupAttachment(SceneComp);
}

// Called when the game starts or when spawned
void ARSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARSBaseWeapon::Interact(ARSDunPlayerCharacter* Interactor)
{
	// �÷��̾��� ���� ���Կ� ���� ĭ���� ���Եǰų� ���� ����� ��ü�ȴ�.

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Interaction ARSBaseWeapon"));
	}
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

