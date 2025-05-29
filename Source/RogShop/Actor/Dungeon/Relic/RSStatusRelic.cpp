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
		// TODO : 플레이어 캐릭터의 현재 HP 증가
	}
	else if (TargetStatus == EStatus::MaxHP)
	{
		// TODO : 플레이어 캐릭터의 최대 HP 증가
	}
	else if (TargetStatus == EStatus::MoveSpeed)
	{
		// TODO : 플레이어 캐릭터의 이동속도 증가
	}
	else if (TargetStatus == EStatus::AttackPower)
	{
		// TODO : 플레이어 캐릭터의 공격력 증가
	}
	else if (TargetStatus == EStatus::AttackSpeed)
	{
		// TODO : 플레이어 캐릭터의 공격속도 증가
	}
}
