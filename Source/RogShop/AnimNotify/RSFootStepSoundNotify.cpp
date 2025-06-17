// Fill out your copyright notice in the Description page of Project Settings.


#include "RSFootStepSoundNotify.h"
#include "RSDunPlayerAnimInstance.h"

void URSFootStepSoundNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
    {
        if (URSDunPlayerAnimInstance* RSAnim = Cast<URSDunPlayerAnimInstance>(AnimInstance))
        {
            RSAnim->PlayFootstepSound();
        }
    }
}