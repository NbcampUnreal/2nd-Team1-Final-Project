// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MonsterData.h"
#include "RSDunPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterProjectileBase.generated.h"

UCLASS()
class ROGSHOP_API AMonsterProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterProjectileBase();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void Fire(const FVector& shootDir);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float damage;
};
