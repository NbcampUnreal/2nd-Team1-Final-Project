// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunSkeletonCharacter.h"

ARSDunSkeletonCharacter::ARSDunSkeletonCharacter()
{
}

void ARSDunSkeletonCharacter::PlayBaseAttackAnim()
{
	PlayAnimMontage(BaseAttackMontage);
	UE_LOG(LogTemp, Warning, TEXT("Skeleton Attack Success!!"));
}

void ARSDunSkeletonCharacter::PlayHitReactAnim()
{
	PlayAnimMontage(HitReactMontage);
	UE_LOG(LogTemp, Warning, TEXT("Skeleton HitReact Success!!"));
}

void ARSDunSkeletonCharacter::PlayDeathAnim()
{
	PlayAnimMontage(DeathMontage);
	UE_LOG(LogTemp, Warning, TEXT("Skeleton Death Success!!"));
}
