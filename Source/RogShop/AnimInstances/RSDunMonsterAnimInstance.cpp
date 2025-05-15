// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunMonsterAnimInstance.h"
#include "RSDunMonsterCharacter.h"

URSDunMonsterAnimInstance::URSDunMonsterAnimInstance()
{
    MonsterCharacter = nullptr;

    Velocity = FVector::ZeroVector;
    GroundSpeed = 0.f;
    bShouldMove = false;
    bIsDead = false;
}

void URSDunMonsterAnimInstance::NativeInitializeAnimation()
{
    AActor* Owner = GetOwningActor();
    if (Owner)
    {
        MonsterCharacter = Cast<ARSDunMonsterCharacter>(Owner);
    }
}

void URSDunMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    if (MonsterCharacter)
    {
        Velocity = MonsterCharacter->GetVelocity();
        GroundSpeed = Velocity.Size2D();
        bShouldMove = GroundSpeed > 5.f;
        // bIsDead =  << TODO :: ���߿� ���� ü�� ��������
        // �ʿ��ϸ� �߰� ���� ó�� (ex. ���� ��, ���� �� ��)
    }
}
