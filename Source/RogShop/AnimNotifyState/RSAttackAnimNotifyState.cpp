// Fill out your copyright notice in the Description page of Project Settings.


#include "RSAttackAnimNotifyState.h"
#include "RogShop/UtilDefine.h"
#include "RSDunPlayerCharacter.h"
#include "RSPlayerWeaponComponent.h"
#include "RSBaseWeapon.h"

void URSAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	RS_LOG_C("RSAttackAnimNotifyState Begin", FColor::Blue);

	// 현재 무기 Trail 켜기
	ARSDunPlayerCharacter* PlayerCharacter = MeshComp->GetOwner<ARSDunPlayerCharacter>();
	if (!PlayerCharacter)
	{
		return;
	}

	URSPlayerWeaponComponent* WeaponComp = PlayerCharacter->GetRSPlayerWeaponComponent();
	if (WeaponComp)
	{
		WeaponComp->StartTrail();
	}
}

void URSAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	ARSDunPlayerCharacter* PlayerCharacter = MeshComp->GetOwner<ARSDunPlayerCharacter>();
	if (!PlayerCharacter)
	{
		return;
	}

	URSPlayerWeaponComponent* WeaponComp = PlayerCharacter->GetRSPlayerWeaponComponent();
	if (WeaponComp)
	{
		WeaponComp->PerformBoxSweepAttack();
	}
}

void URSAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	RS_LOG_C("RSAttackAnimNotifyState End", FColor::Orange);

	ARSDunPlayerCharacter* PlayerCharacter = MeshComp->GetOwner<ARSDunPlayerCharacter>();
	if (!PlayerCharacter)
	{
		return;
	}

	URSPlayerWeaponComponent* WeaponComp = PlayerCharacter->GetRSPlayerWeaponComponent();
	if (WeaponComp)
	{
		// 현재 무기 Trail 끄기
		WeaponComp->EndTrail();

		// 데미지를 가했던 액터들을 저장한 배열 초기화
		WeaponComp->ResetDamagedActors();
	}
}
