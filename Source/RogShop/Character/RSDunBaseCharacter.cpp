// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ARSDunBaseCharacter::ARSDunBaseCharacter()
{	
	MoveSpeed = 600.f;
	MaxHP = 100.f;
	HP = MaxHP;

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	bIsDead = false;
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

void ARSDunBaseCharacter::IncreaseMaxHP(float Amount)
{
	float NewMaxHP = Amount + MaxHP;
	MaxHP = NewMaxHP;
}

void ARSDunBaseCharacter::DecreaseMaxHP(float Amount)
{
	float NewMaxHP = FMath::Clamp(MaxHP - Amount, 0.0f, MaxHP);
	MaxHP = NewMaxHP;

	// ���� ü���� �� ���� ��� �ִ�ü������ ����
	if (HP > MaxHP)
	{
		HP = MaxHP;
	}

	// ���� ü���� 0���ϰ� �� ��� ��� ó��
	if (HP <= 0.f)
	{
		OnDeath();
	}
}

float ARSDunBaseCharacter::GetHP() const
{
	return HP;
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

	// ���� ü���� 0���ϰ� �� ��� ��� ó��
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
	bIsDead = true;
}
