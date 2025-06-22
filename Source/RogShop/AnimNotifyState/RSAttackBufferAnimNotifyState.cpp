// Fill out your copyright notice in the Description page of Project Settings.


#include "RSAttackBufferAnimNotifyState.h"
#include "RSDunPlayerCharacter.h"
#include "RSPlayerWeaponComponent.h"
#include "RogShop/UtilDefine.h"

URSAttackBufferAnimNotifyState::URSAttackBufferAnimNotifyState()
{
	bNextAnimStart = false;
}

void URSAttackBufferAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	RS_LOG_C("RSAttackBufferAnimNotifyState Begin", FColor::Blue);

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

	RS_LOG_C("RSAttackBufferAnimNotifyState End", FColor::Blue);

	if (!MeshComp)
	{
		return;
	}

	ARSDunPlayerCharacter* PlayerCharacter = MeshComp->GetOwner<ARSDunPlayerCharacter>();
	if (!PlayerCharacter)
	{
		return;
	}

	// 다음 콤보 공격을 진행해야하며, 몽타주를 스킵한 상태가 아닌 경우
	if (bNextAnimStart && !PlayerCharacter->IsSkippingMontage())
	{
		return;
	}

	URSPlayerWeaponComponent* WeaponComp = PlayerCharacter->GetRSPlayerWeaponComponent();
	if (WeaponComp)
	{
		WeaponComp->ResetCombo();
	}
}
