// Fill out your copyright notice in the Description page of Project Settings.


#include "RSKillStatusRelic.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RSSpawnManagerAccessor.h"
#include "RSDunBaseCharacter.h"
#include "RSDunPlayerCharacter.h"

void URSKillStatusRelic::ApplyEffect(ARSDunPlayerCharacter* OwnerCharacter)
{
	if (!OwnerCharacter)
	{
		return;
	}

	IRSSpawnManagerAccessor* SpawnManagerAccessor = GetWorld()->GetAuthGameMode<IRSSpawnManagerAccessor>();
	if (SpawnManagerAccessor)
	{
		URSSpawnManager* SpawnManager = SpawnManagerAccessor->GetSpawnManager();
		if (SpawnManager)
		{
			SpawnManager->OnEnemySpawn.AddDynamic(this, &URSKillStatusRelic::BindFunc);
		}
	}

	PlayerCharacter = OwnerCharacter;
}

void URSKillStatusRelic::LoadEffect(ARSDunPlayerCharacter* OwnerCharacter)
{
	ApplyEffect(OwnerCharacter);
}

void URSKillStatusRelic::BindFunc(ARSDunBaseCharacter* TargetCharacter)
{
	TargetCharacter->OnCharacterDied.AddDynamic(this, &URSKillStatusRelic::KillEffectStatus);
}

void URSKillStatusRelic::KillEffectStatus(ARSDunBaseCharacter* DeadCharacter)
{
	if (!PlayerCharacter.IsValid())
	{
		return;
	}

	// 로직 실행 확률이 더 낮은 경우 실행 취소
	int32 Rand = FMath::RandRange(1, 100);
	if (Rand > Persent)
	{
		return;
	}

	if (TargetStatus == ECharacterStatus::HP)
	{
		PlayerCharacter->IncreaseHP(Amount);
	}
	else if (TargetStatus == ECharacterStatus::MaxHP)
	{
		PlayerCharacter->IncreaseMaxHP(Amount);
	}
	else if (TargetStatus == ECharacterStatus::MoveSpeed)
	{
		UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
		if (MovementComp)
		{
			float CurrentSpeed = MovementComp->GetMaxSpeed();
			CurrentSpeed += Amount;
			PlayerCharacter->ChangeMoveSpeed(CurrentSpeed);
		}
	}
	else if (TargetStatus == ECharacterStatus::AttackPower)
	{
		PlayerCharacter->IncreaseAttackPower(Amount);
	}
	else if (TargetStatus == ECharacterStatus::AttackSpeed)
	{
		PlayerCharacter->IncreaseAttackSpeed(Amount);
	}
}