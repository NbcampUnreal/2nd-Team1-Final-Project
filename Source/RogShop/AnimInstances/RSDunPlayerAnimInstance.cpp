// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URSDunPlayerAnimInstance::URSDunPlayerAnimInstance()
{
	Character = nullptr;
	CharacterMovement = nullptr;

	Velocity = FVector::ZeroVector;
	GroundSpeed = 0.f;
	bShouldMove = false;
	bIsFalling = false;
}

void URSDunPlayerAnimInstance::NativeInitializeAnimation()
{
	Character = Cast<ACharacter>(GetOwningActor());

	if (Character)
	{
		CharacterMovement = Character->GetCharacterMovement();
	}
}

void URSDunPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Character && CharacterMovement)
	{
		Velocity = CharacterMovement->GetLastUpdateVelocity();
		GroundSpeed = Velocity.Size2D();
		bShouldMove = (!FMath::IsNearlyZero(CharacterMovement->GetCurrentAcceleration().SizeSquared()) && 3.f <= GroundSpeed) ? true : false;
		bIsFalling = CharacterMovement->IsFalling();
	}
}
