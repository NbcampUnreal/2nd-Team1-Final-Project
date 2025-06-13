// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunMonsterAnimInstance.h"
#include "RSDunMonsterCharacter.h"

URSDunMonsterAnimInstance::URSDunMonsterAnimInstance()
{
    MonsterCharacter = nullptr;

    Velocity = FVector::ZeroVector;
    GroundSpeed = 0.f;
    Speed = 0.f;
    Direction = 0.f;
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
        // 1. 현재 이동 속도, 방향, 회전 raw한 값 구하기
        Velocity = MonsterCharacter->GetVelocity();
        FRotator ActorRotation = MonsterCharacter->GetActorRotation();
        GroundSpeed = Velocity.Size2D();

        // 2. 보간된 Speed 값 적용 (이동 부드럽게)
        Speed = FMath::FInterpTo(Speed, GroundSpeed, DeltaSeconds, 6.0f);

        // 3. 이동 여부 판단
        bShouldMove = GroundSpeed > 5.f;

        // 4. 방향 계산 및 보간 (회전 부드럽게)
        FVector SmoothedVelocity = FMath::VInterpTo(SmoothedVelocity, Velocity, DeltaSeconds, 6.0f);
        //FRotator SmoothedRotation = FMath::RInterpTo(SmoothedRotation, ActorRotation, DeltaSeconds, 6.0f);
        float TargetDirection = CalculateDirection(SmoothedVelocity, ActorRotation);
        Direction = FMath::FInterpTo(Direction, TargetDirection, DeltaSeconds, 6.0f);
    }
}
