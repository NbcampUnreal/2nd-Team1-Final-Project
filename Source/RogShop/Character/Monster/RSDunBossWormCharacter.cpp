// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunBossWormCharacter.h"

ARSDunBossWormCharacter::ARSDunBossWormCharacter()
{
}

void ARSDunBossWormCharacter::PlayBaseAttackAnim()
{
	PlayAnimMontage(BaseAttackMontage);
	UE_LOG(LogTemp, Warning, TEXT("BossWorm Attack Success!!"));
}

void ARSDunBossWormCharacter::PlayHitReactAnim()
{
	PlayAnimMontage(HitReactMontage);
	UE_LOG(LogTemp, Warning, TEXT("BossWorm HitReact Success!!"));
}

void ARSDunBossWormCharacter::PlayDeathAnim()
{
	PlayAnimMontage(DeathMontage);
	UE_LOG(LogTemp, Warning, TEXT("BossWorm Death Success!!"));
}
