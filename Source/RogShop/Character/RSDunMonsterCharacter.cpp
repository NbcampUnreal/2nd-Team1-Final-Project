// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunMonsterCharacter.h"

ARSDunMonsterCharacter::ARSDunMonsterCharacter()
{
}

void ARSDunMonsterCharacter::PlayBaseAttackAnim()
{
	PlayAnimMontage(BaseAttackMontage);
	UE_LOG(LogTemp, Warning, TEXT("Attack Complete!!"));
}

void ARSDunMonsterCharacter::PlayHitReactAnim()
{
	PlayAnimMontage(HitReactMontage);
	UE_LOG(LogTemp, Warning, TEXT("HitReact Complete!!"));
}

void ARSDunMonsterCharacter::PlayDeathAnim()
{
	PlayAnimMontage(DeathMontage);
	UE_LOG(LogTemp, Warning, TEXT("Death Complete!!"));
}
