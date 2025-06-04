// Fill out your copyright notice in the Description page of Project Settings.


#include "RSStatusRelic.h"
#include "RSDunPlayerCharacter.h"

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
		// 해당 이동속도로 고정됨
		OwnerCharacter->ChangeMoveSpeed(Amount);
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
