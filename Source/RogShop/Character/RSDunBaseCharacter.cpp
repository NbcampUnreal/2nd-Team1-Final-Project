// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunBaseCharacter.h"

ARSDunBaseCharacter::ARSDunBaseCharacter()
{
	MaxHP = 100.f;
	HP = MaxHP;

	bIsDead = false;
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
}

bool ARSDunBaseCharacter::GetIsDead()
{
	return bIsDead;
}

void ARSDunBaseCharacter::OnDeath()
{
	bIsDead = true;
}
