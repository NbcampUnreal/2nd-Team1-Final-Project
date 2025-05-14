// Fill out your copyright notice in the Description page of Project Settings.


#include "RSAttackBufferAnimNotifyState.h"
#include "RSDunPlayerCharacter.h"
#include "RSPlayerWeaponComponent.h"

URSAttackBufferAnimNotifyState::URSAttackBufferAnimNotifyState()
{
	bNextAnimStart = false;
}

void URSAttackBufferAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	bNextAnimStart = false;
}

void URSAttackBufferAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (!MeshComp)
	{
		return;
	}

	ARSDunPlayerCharacter* PlayerCharacter = MeshComp->GetOwner<ARSDunPlayerCharacter>();
	if (!PlayerCharacter)
	{
		return;
	}

	URSPlayerWeaponComponent* WeaponComp = PlayerCharacter->GetRSPlayerWeaponComponent();
	if (WeaponComp && WeaponComp->ContinueComboAttack())
	{
		bNextAnimStart = true;
	}
}

void URSAttackBufferAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (bNextAnimStart)
	{
		return;
	}

	if (!MeshComp)
	{
		return;
	}

	ARSDunPlayerCharacter* PlayerCharacter = MeshComp->GetOwner<ARSDunPlayerCharacter>();
	if (!PlayerCharacter)
	{
		return;
	}

	URSPlayerWeaponComponent* WeaponComp = PlayerCharacter->GetRSPlayerWeaponComponent();
	if (WeaponComp)
	{
		WeaponComp->ResetCombo();
	}
}
