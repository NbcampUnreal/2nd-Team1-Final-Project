// Fill out your copyright notice in the Description page of Project Settings.


#include "RSStatusRelic.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void URSStatusRelic::ApplyEffect(ARSDunPlayerCharacter* OwnerCharacter)
{
	if (!OwnerCharacter)
	{
		return;
	}

	if (TargetStatus == EStatus::HP)
	{
		OwnerCharacter->IncreaseHP(Amount);
	}
	else if (TargetStatus == EStatus::MaxHP)
	{
		OwnerCharacter->IncreaseMaxHP(Amount);
	}
	else if (TargetStatus == EStatus::MoveSpeed)
	{
		UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement();
		if (MovementComp)
		{
			float CurrentSpeed = MovementComp->GetMaxSpeed();
			CurrentSpeed + Amount;
			OwnerCharacter->ChangeMoveSpeed(CurrentSpeed);
		}
	}
	else if (TargetStatus == EStatus::AttackPower)
	{
		OwnerCharacter->IncreaseAttackPower(Amount);
	}
	else if (TargetStatus == EStatus::AttackSpeed)
	{
		OwnerCharacter->IncreaseAttackSpeed(Amount);
	}
}
