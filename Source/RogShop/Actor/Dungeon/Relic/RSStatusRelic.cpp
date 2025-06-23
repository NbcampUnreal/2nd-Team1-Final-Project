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

	if (TargetStatus == ECharacterStatus::HP)
	{
		OwnerCharacter->IncreaseHP(Amount);
	}
	else if (TargetStatus == ECharacterStatus::MaxHP)
	{
		OwnerCharacter->IncreaseMaxHP(Amount);
	}
	else if (TargetStatus == ECharacterStatus::MoveSpeed)
	{
		UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement();
		if (MovementComp)
		{
			float CurrentSpeed = MovementComp->GetMaxSpeed();
			CurrentSpeed += Amount;
			OwnerCharacter->ChangeMoveSpeed(CurrentSpeed);
		}
	}
	else if (TargetStatus == ECharacterStatus::AttackPower)
	{
		OwnerCharacter->IncreaseAttackPower(Amount);
	}
	else if (TargetStatus == ECharacterStatus::AttackSpeed)
	{
		OwnerCharacter->IncreaseAttackSpeed(Amount);
	}
}

void URSStatusRelic::LoadEffect(ARSDunPlayerCharacter* OwnerCharacter)
{
	if (TargetStatus == ECharacterStatus::HP)
	{
		// HP를 회복시키는 개념이므로 로드시 해당 기능을 제외한 스탯 상승 부분만 적용한다.
		return;
	}
	else
	{
		ApplyEffect(OwnerCharacter);
	}
}
