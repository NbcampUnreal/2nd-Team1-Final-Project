// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterProjectileBase.h"
#include "Engine/EngineTypes.h"

// Sets default values
AMonsterProjectileBase::AMonsterProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

    bulletRadius = 25.0f;
    bulletInitSpeed = 1000.f;
    bulletMaxSpeed = 1000.f;
    bulletGravity = 1.f;

    Scene= CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(Scene);

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("BulletCollision"));
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionComponent->SetupAttachment(Scene);

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonsterProjectileBase::BulletOverlapBegin);

    // 이동 컴포넌트 생성
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = CollisionComponent;
    ProjectileMovement->InitialSpeed = bulletInitSpeed;
    ProjectileMovement->MaxSpeed = bulletMaxSpeed;
    ProjectileMovement->bRotationFollowsVelocity = false;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = bulletGravity;

    // 수명 설정
    InitialLifeSpan = 3.0f;

    damage = 5.f;
}

void AMonsterProjectileBase::BulletOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        bool bPlayerOverlap = IsValid(OtherActor) && OtherActor->IsA(ARSDunPlayerCharacter::StaticClass());

        if (bPlayerOverlap)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("원거리 피격"));
            UGameplayStatics::ApplyDamage(OtherActor, damage, GetInstigator()->GetController(), this, nullptr);
        }
    }
}

void AMonsterProjectileBase::Fire(const FVector& shotDir)
{
    ProjectileMovement->Velocity = shotDir * ProjectileMovement->InitialSpeed;
}