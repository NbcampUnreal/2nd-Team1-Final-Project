// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunBaseCharacter.h"
#include "NavigationInvokerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Engine/TargetPoint.h"
#include "Engine/OverlapResult.h"
#include "TimerManager.h"
#include "RSDunMonsterCharacter.generated.h"

class ARSMonsterAIController;
class UMeleeAttackBoxComponent;

UCLASS()
class ROGSHOP_API ARSDunMonsterCharacter : public ARSDunBaseCharacter
{
	GENERATED_BODY()

public:
	ARSDunMonsterCharacter();

	virtual void BeginPlay()override;

	// 애니메이션 실행 함수
	virtual void PlayBaseAttackAnim();
	virtual void PlayHitReactAnim();
	virtual void PlayDeathAnim();

	//Navigation Invoker function
	FORCEINLINE class UNavigationInvokerComponent* GetNavInvoker() const { return navInvoker; };

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Trace")
	virtual void PerformAttackTrace() PURE_VIRTUAL(ARSDunMonsterCharacter::PerformAttackTrace, );

	//NavLink jump function
	UFUNCTION(BlueprintCallable)
	void JumpTo(FVector destination);

	//Patrol
	UFUNCTION(BlueprintCallable)
	void FindNearPatrolPoint();

	UFUNCTION()
	TArray<AActor*> GetPatrolPoint();

	bool HasAttackTraced() const;
	void SetAttackTraced(bool bNewValue);

protected:
	UFUNCTION(BlueprintCallable, Category = "Enemy|Status")
	void OnDeath();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UMeleeAttackBoxComponent> MeleeAttackBoxComponent;

protected:
	// 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> BaseAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DeathMontage;

	//NavInvoker
	UPROPERTY(BlueprintReadWrite, Category = Navigation, meta = (AllowPrivateAccess = "true"))//Navigation Invoker Setting
	UNavigationInvokerComponent* navInvoker;

	UPROPERTY(BlueprintReadWrite, Category = Navigation)
	float navGenerationRadius;

	UPROPERTY(BlueprintReadWrite, Category = Navigation)
	float navRemovalRadius;

	//NavLink
	UPROPERTY(BlueprintReadWrite, Category = Navigation)
	float jumpForce;

	//Patrol
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite, Category = "Patrol")
	TArray<AActor*> patrolPoints;

	UPROPERTY(BlueprintReadWrite, Category = "Patrol")
	float maxDetectPatrolRoute;

	FTimerHandle detectDelayTimer;

	bool bHasAttackTraced;  // 트레이스(몬스터한테 공격 판정이)가 되었는지 판단하는 변수

private:
	TObjectPtr<ARSMonsterAIController> AIController;  // TODO : 혹시나 캐싱해서 쓸 일 생길까봐 미리 만들어둠.

};
