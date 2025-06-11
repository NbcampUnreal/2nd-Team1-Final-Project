// Fill out your copyright notice in the Description page of Project Settings.

#include "RSTycoonAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void URSTycoonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (APawn* Owner = TryGetPawnOwner())
    {
        ACharacter* Character = Cast<ACharacter>(Owner);

        if (Character && Character->GetCharacterMovement())
        {
            FVector Velocity = Character->GetCharacterMovement()->Velocity;
            Speed = Velocity.Size();

            if (Speed >= 3.f)
            {
                bShouldMove = true;
            }
            else
            {
                bShouldMove = false;
            }
        }
    }
}
