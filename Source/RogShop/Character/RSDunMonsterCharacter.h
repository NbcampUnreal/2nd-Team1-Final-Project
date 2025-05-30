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

USTRUCT(BlueprintType)
struct FMonsterAttackTraceData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector TraceBoxHalfSize;	// 트레이스 반경 (옆으로 얼마만큼 쏠지)

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceLength;			// 트레이스 길이 (앞으로 얼마만큼 쏠지)

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceForwardOffset;	// 소켓 시작점 앞뒤 보정

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceRightOffset;     // 소켓 시작점 좌우 보정

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceUpOffset;        // 소켓 시작점 높이 보정

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SocketLocation;		// 트레이스가 쏴질 소켓의 시작점

};

USTRUCT(BlueprintType)
struct FMonsterAttackSkillData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SkillName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 PlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMonsterAttackTraceData AttackTrace;
};

UCLASS()
class ROGSHOP_API ARSDunMonsterCharacter : public ARSDunBaseCharacter
{
	GENERATED_BODY()

public:
	ARSDunMonsterCharacter();

	virtual void BeginPlay()override;

	// 애니메이션 실행 함수
	void PlayAttackAnim();
	void PlayDeathAnim();
	UFUNCTION(BlueprintCallable)
	void PlaySkill_1();
	UFUNCTION(BlueprintCallable)
	void PlaySkill_2();
	UFUNCTION(BlueprintCallable)
	void PlaySkill_3();

	UFUNCTION()
	void OnDeathMontageEnded(UAnimMontage* montage, bool bInterrupted);  //사망 모션이 끝난 경우

	//Navigation Invoker function
	FORCEINLINE class UNavigationInvokerComponent* GetNavInvoker() const { return navInvoker; };

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Trace")
	void PerformAttackTrace(int32 SkillIndex);

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
	void InitMonsterData();

protected:
	// 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMonsterAttackSkillData> MonsterAttackSkills;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> SkillMontage_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> SkillMontage_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> SkillMontage_3;

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

	// 트레이스 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackTrace")
	FVector TraceBoxHalfSize;	// 트레이스 반경 (옆으로 얼마만큼 쏠지)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackTrace")
	float TraceLength;			// 트레이스 길이 (앞으로 얼마만큼 쏠지)

	UPROPERTY(EditAnywhere, Category = "AttackTrace")
	float TraceForwardOffset;	// 소켓 시작점 앞뒤 보정

	UPROPERTY(EditAnywhere, Category = "AttackTrace")
	float TraceRightOffset;     // 소켓 시작점 좌우 보정

	UPROPERTY(EditAnywhere, Category = "AttackTrace")
	float TraceUpOffset;        // 소켓 시작점 높이 보정

	UPROPERTY(EditAnywhere, Category = "AttackTrace")
	FName SocketLocation;		// 트레이스가 쏴질 소켓의 시작점

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName MonsterRowName;			 // 이 몬스터가 참조하는 RowName

	UDataTable* MonsterDataTable;	 // 연결된 데이터 테이블
	FTimerHandle detectDelayTimer;

private:
	TObjectPtr<ARSMonsterAIController> AIController;  // TODO : 혹시나 캐싱해서 쓸 일 생길까봐 미리 만들어둠.

};
