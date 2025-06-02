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
#include "MonsterData.h"
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
	void PlayAttackAnim();
	void PlayDeathAnim();
	UFUNCTION(BlueprintCallable)
	void PlaySkill_1();
	UFUNCTION(BlueprintCallable)
	void PlaySkill_2();
	UFUNCTION(BlueprintCallable)
	void PlaySkill_3();
	UFUNCTION(BlueprintCallable)
	void AIAction(int32 actionIdx);

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> SkillMontage_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> SkillMontage_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> SkillMontage_3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 skillActionIdx;//어떤 스킬을 시전하는 중인지 저장, PerformActionTrace 에서 해당 변수를 참조해 행동하도록 함

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName MonsterRowName;			 // 이 몬스터가 참조하는 RowName

	FTimerHandle detectDelayTimer;

	// 데이터 테이블 관련
	UDataTable* MonsterDataTable;	 // 연결된 데이터 테이블
	TArray<FMonsterAttackSkillData> MonsterAttackSkills;	// 몬스터 공격 스킬을 모아놓은 구조체를 배열로 저장
	TArray<FMonsterAttackTraceData> CachedAttackTraceDataArray;	// 공격 트레이스를 캐싱해두고 다른 스킬 사용시 인덱스에서 꺼내 쓰는 용도

private:
	TObjectPtr<ARSMonsterAIController> AIController;  // TODO : 혹시나 캐싱해서 쓸 일 생길까봐 미리 만들어둠.

};
