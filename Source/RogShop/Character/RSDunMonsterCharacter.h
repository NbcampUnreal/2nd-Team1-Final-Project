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
#include "Animation/AnimInstance.h"
#include "RSDunMonsterCharacter.generated.h"

class ARSMonsterAIController;

UCLASS()
class ROGSHOP_API ARSDunMonsterCharacter : public ARSDunBaseCharacter
{
	GENERATED_BODY()

public:
	ARSDunMonsterCharacter();

	virtual void BeginPlay()override;

	// 애니메이션 실행 함수
	virtual void PlayBaseAttackAnim();
	virtual void PlayDeathAnim();

	UFUNCTION()
	void OnDeathMontageEnded(UAnimMontage* montage, bool bInterrupted);//사망 모션이 끝난 경우

	//Navigation Invoker function
	FORCEINLINE class UNavigationInvokerComponent* GetNavInvoker() const { return navInvoker; };

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Trace")
	virtual void PerformAttackTrace();

	//NavLink jump function
	UFUNCTION(BlueprintCallable)
	void JumpTo(FVector destination);

	//Patrol
	UFUNCTION(BlueprintCallable)
	void FindNearPatrolPoint();

	UFUNCTION()
	TArray<AActor*> GetPatrolPoint();

	UFUNCTION(BlueprintCallable, Category = "Enemy|Status")
	void OnDeath();

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

	// 트레이스 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackTrace")
	FVector TraceBoxHalfSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackTrace")
	float TraceLength;        // 트레이스 길이 (앞으로 얼마만큼 쏠지)

	UPROPERTY(EditAnywhere, Category = "AttackTrace")
	float TraceForwardOffset; // 소켓 기준 앞으로 얼마나 밀지

	UPROPERTY(EditAnywhere, Category = "AttackTrace")
	float TraceRightOffset;     // (선택) 좌우 보정

	UPROPERTY(EditAnywhere, Category = "AttackTrace")
	float TraceUpOffset;        // (선택) 높이 보정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackTrace")
	FName SocketLocation;		// 트레이스가 나갈 소켓의 시작점

	FTimerHandle detectDelayTimer;

private:
	TObjectPtr<ARSMonsterAIController> AIController;  // TODO : 혹시나 캐싱해서 쓸 일 생길까봐 미리 만들어둠.

};
