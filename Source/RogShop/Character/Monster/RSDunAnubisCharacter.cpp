// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunAnubisCharacter.h"

ARSDunAnubisCharacter::ARSDunAnubisCharacter()
{
}

void ARSDunAnubisCharacter::PlayBaseAttackAnim()
{
	PlayAnimMontage(BaseAttackMontage);
	UE_LOG(LogTemp, Warning, TEXT("Anubis Attack Success!!"));
}

void ARSDunAnubisCharacter::PlayHitReactAnim()
{
	PlayAnimMontage(HitReactMontage);
	UE_LOG(LogTemp, Warning, TEXT("Anubis HitReact Success!!"));
}

void ARSDunAnubisCharacter::PlayDeathAnim()
{
	PlayAnimMontage(DeathMontage);
	UE_LOG(LogTemp, Warning, TEXT("Anubis Death Success!!"));
}
