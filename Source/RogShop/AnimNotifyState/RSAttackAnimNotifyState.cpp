// Fill out your copyright notice in the Description page of Project Settings.


#include "RSAttackAnimNotifyState.h"
#include "RSDunPlayerCharacter.h"
#include "RSPlayerWeaponComponent.h"
#include "RSBaseWeapon.h"

void URSAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	// �浹 �ݸ��� ����
	ARSDunPlayerCharacter* PlayerCharacter = MeshComp->GetOwner<ARSDunPlayerCharacter>();
	if (!PlayerCharacter)
	{
		return;
	}

	URSPlayerWeaponComponent* WeaponComp = PlayerCharacter->GetRSPlayerWeaponComponent();
	if (WeaponComp)
	{
		WeaponComp->StartAttackOverlap();
	}
}

void URSAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	
}

void URSAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	// �浹 �ݸ��� ���� ����
	ARSDunPlayerCharacter* PlayerCharacter = MeshComp->GetOwner<ARSDunPlayerCharacter>();
	if (!PlayerCharacter)
	{
		return;
	}

	URSPlayerWeaponComponent* WeaponComp = PlayerCharacter->GetRSPlayerWeaponComponent();
	if (WeaponComp)
	{
		WeaponComp->EndAttackOverlap();
	}
}
