// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunBossSpiderQueenCharacter.h"

ARSDunBossSpiderQueenCharacter::ARSDunBossSpiderQueenCharacter()
{
}

void ARSDunBossSpiderQueenCharacter::PlayBaseAttackAnim()
{
	PlayAnimMontage(BaseAttackMontage);
	UE_LOG(LogTemp, Warning, TEXT("Boss SpiderQueen Attack Success!!"));
}

void ARSDunBossSpiderQueenCharacter::PlayHitReactAnim()
{
	PlayAnimMontage(HitReactMontage);
	UE_LOG(LogTemp, Warning, TEXT("Boss SpiderQueen HitReact Success!!"));
}

void ARSDunBossSpiderQueenCharacter::PlayDeathAnim()
{
	PlayAnimMontage(DeathMontage);
	UE_LOG(LogTemp, Warning, TEXT("Boss SpiderQueen Death Success!!"));
}
