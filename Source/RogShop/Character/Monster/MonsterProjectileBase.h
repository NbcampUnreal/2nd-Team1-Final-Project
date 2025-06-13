// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CollisionQueryParams.h"
#include "MonsterData.h"
#include "RSDunPlayerCharacter.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/RSMonsterAttackTraceDefine.h"
#include "MonsterProjectileBase.generated.h"

UCLASS()
class ROGSHOP_API AMonsterProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterProjectileBase();

	UFUNCTION()
	void BulletOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Fire(const FVector& shootDir);

//	virtual void Tick(float DeltaTime)override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	USceneComponent* Scene;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float damage;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float bulletRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float bulletInitSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float bulletMaxSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float bulletGravity;
};
