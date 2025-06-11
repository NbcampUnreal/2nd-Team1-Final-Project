// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "RSDunPlayerCharacter.h"
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

void URSDunPlayerAnimInstance::PlayFootstepSound() //발소리 재생 함수
{
	if (!Character)
	{
		return;
	}

	FVector Start = Character->GetActorLocation();
	FVector End = Start - FVector(0, 0, 50);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);

	if (Character->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		UPhysicalMaterial* PhysMat = HitResult.PhysMaterial.Get();
		if (PhysMat)
		{
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysMat);

			// 캐릭터 클래스에서 발소리 맵 가져오기
			ARSDunPlayerCharacter* Player = Cast<ARSDunPlayerCharacter>(Character);
			if (Player)
			{
				if (USoundBase* FootstepSound = Player->GetFootstepSound(SurfaceType))
				{
					UGameplayStatics::PlaySoundAtLocation(Player, FootstepSound, Player->GetActorLocation());
				}
			}
		}
	}

}
