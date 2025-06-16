// Fill out your copyright notice in the Description page of Project Settings.


#include "RSWeaponChangeAnimNotify.h"
#include "RSDunPlayerCharacter.h"
#include "RSPlayerWeaponComponent.h"

void URSWeaponChangeAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

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
		WeaponComp->EquipWeaponToCharacter();
	}
}
