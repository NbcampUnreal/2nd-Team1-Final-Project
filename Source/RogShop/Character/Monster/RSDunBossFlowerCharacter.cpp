// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunBossFlowerCharacter.h"

ARSDunBossFlowerCharacter::ARSDunBossFlowerCharacter()
{
}

void ARSDunBossFlowerCharacter::PlayBaseAttackAnim()
{
	PlayAnimMontage(BaseAttackMontage);
	UE_LOG(LogTemp, Warning, TEXT("BossFlower Attack Success!!"));
}

void ARSDunBossFlowerCharacter::PlayHitReactAnim()
{
	PlayAnimMontage(HitReactMontage);
	UE_LOG(LogTemp, Warning, TEXT("BossFlower HitReact Success!!"));
}

void ARSDunBossFlowerCharacter::PlayDeathAnim()
{
	PlayAnimMontage(DeathMontage);
	UE_LOG(LogTemp, Warning, TEXT("BossFlower Death Success!!"));
}
