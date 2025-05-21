// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunBoarCharacter.h"

ARSDunBoarCharacter::ARSDunBoarCharacter()
{
}

void ARSDunBoarCharacter::PlayBaseAttackAnim()
{
	PlayAnimMontage(BaseAttackMontage);
	UE_LOG(LogTemp, Warning, TEXT("Boar Attack Success!!"));
}

void ARSDunBoarCharacter::PlayHitReactAnim()
{
	PlayAnimMontage(HitReactMontage);
	UE_LOG(LogTemp, Warning, TEXT("Boar HitReact Success!!"));
}

void ARSDunBoarCharacter::PlayDeathAnim()
{
	PlayAnimMontage(DeathMontage);
	UE_LOG(LogTemp, Warning, TEXT("Boar Death Success!!"));
}