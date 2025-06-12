// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterProjectileBase.h"

// Sets default values
AMonsterProjectileBase::AMonsterProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
    CollisionComponent->OnComponentHit.AddDynamic(this, &AMonsterProjectileBase::OnHit);
    RootComponent = CollisionComponent;

//    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
//    ProjectileMesh->SetupAttachment(RootComponent);

    // 이동 컴포넌트 생성
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = CollisionComponent;
    ProjectileMovement->InitialSpeed = 2000.f;
    ProjectileMovement->MaxSpeed = 2000.f;
    ProjectileMovement->bRotationFollowsVelocity = false;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 1.f;

    // 수명 설정
    InitialLifeSpan = 3.0f;

    damage = 5.f;
}

void AMonsterProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && OtherComponent)
    {
        FHitResult HitResult;
        AActor* HitActor = HitResult.GetActor();
        bool bPlayerHit = IsValid(HitActor) && HitActor->IsA(ARSDunPlayerCharacter::StaticClass());
        
        if (bPlayerHit)
        {
            UGameplayStatics::ApplyDamage(HitActor, damage, GetInstigator()->GetController(), this, nullptr);
        }

        // 투사체 제거
        Destroy();
    }
}

void AMonsterProjectileBase::Fire(const FVector& shotDir)
{
    ProjectileMovement->Velocity = shotDir * ProjectileMovement->InitialSpeed;
}