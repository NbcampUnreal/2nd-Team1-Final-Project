// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

ARSDunBaseCharacter::ARSDunBaseCharacter()
{	
	MaxHP = 100.f;
	HP = MaxHP;
	MoveSpeed = 600.f;

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	bIsDead = false;
}

float ARSDunBaseCharacter::GetMoveSpeed() const
{
	return MoveSpeed;
}

void ARSDunBaseCharacter::ChangeMoveSpeed(float Amount)
{
	MoveSpeed = Amount;

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp)
	{
		MovementComp->MaxWalkSpeed = MoveSpeed;
	}
}

float ARSDunBaseCharacter::GetMaxHP() const
{
	return MaxHP;
}

void ARSDunBaseCharacter::ChangeMaxHP(float Amount)
{
	float NewMaxHP = FMath::Max(Amount, 0.0f);
	MaxHP = NewMaxHP;

	// 현재 체력이 더 많은 경우 최대체력으로 갱신
	if (HP > MaxHP)
	{
		HP = MaxHP;
	}

	// 현재 체력이 0이하가 된 경우 사망 처리
	if (HP <= 0.f)
	{
		OnDeath();
	}
}

void ARSDunBaseCharacter::IncreaseMaxHP(float Amount)
{
	float NewMaxHP = Amount + MaxHP;
	MaxHP = NewMaxHP;
}

void ARSDunBaseCharacter::DecreaseMaxHP(float Amount)
{
	float NewMaxHP = FMath::Clamp(MaxHP - Amount, 0.0f, MaxHP);
	MaxHP = NewMaxHP;

	// 현재 체력이 더 많은 경우 최대체력으로 갱신
	if (HP > MaxHP)
	{
		HP = MaxHP;
	}

	// 현재 체력이 0이하가 된 경우 사망 처리
	if (HP <= 0.f)
	{
		OnDeath();
	}
}

float ARSDunBaseCharacter::GetHP() const
{
	return HP;
}

void ARSDunBaseCharacter::ChangeHP(float Amount)
{
	float NewHP = FMath::Clamp(Amount, 0.0f, MaxHP);
	HP = NewHP;

	// 현재 체력이 0이하가 된 경우 사망 처리
	if (HP <= 0.f)
	{
		OnDeath();
	}
}

void ARSDunBaseCharacter::IncreaseHP(float Amount)
{
	float NewHP = FMath::Clamp(HP + Amount, 0.0f, MaxHP);
	HP = NewHP;
}

void ARSDunBaseCharacter::DecreaseHP(float Amount)
{
	float NewHP = FMath::Clamp(HP - Amount, 0.0f, MaxHP);
	HP = NewHP;

	// 현재 체력이 0이하가 된 경우 사망 처리
	if (HP <= 0.f)
	{
		OnDeath();
	}
}

bool ARSDunBaseCharacter::GetIsDead()
{
	return bIsDead;
}

void ARSDunBaseCharacter::OnDeath()
{
	OnCharacterDied.Broadcast(this);

	bIsDead = true;

	// 레벨 오브젝트를 제외한 모든 오브젝트와 충돌하지 않도록 콜리전 설정 변경
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeadCharacter"));
	GetMesh()->SetCollisionProfileName(TEXT("DeadCharacter"));

	// 사망 애니메이션 재생
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(DeathMontage);
	}
}
