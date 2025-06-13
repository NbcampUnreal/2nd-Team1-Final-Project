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
#include "MonsterProjectileBase.h"
#include "RSDunMonsterCharacter.generated.h"

class ARSMonsterAIController;
class UWidgetComponent;

UCLASS()
class ROGSHOP_API ARSDunMonsterCharacter : public ARSDunBaseCharacter
{
	GENERATED_BODY()

public:
	ARSDunMonsterCharacter();

	virtual void BeginPlay()override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void PlayAction(int32 actionIdx, FVector interestedPos);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UtillitySkill(int32 actionIdx, FVector interestedPos);
	void UtillitySkill_Implementation(int32 actionIdx, FVector interestedPos);

	UFUNCTION(BlueprintCallable)
	int32 GetActionLength();//컨트롤러에 캐릭터가 몇가지의 액션을 가지고 있는지 반환

	UFUNCTION(BlueprintCallable)
	bool GetIsMeleeSkill(int32 actionIdx);//근접공격 여부 판별, 컨트롤러 전달

	UFUNCTION(BlueprintCallable)
	bool GetIsPlayingAnim();//몽타주가 실행중인지 

	UFUNCTION(BlueprintCallable)
	float GetAtkRange();
	UFUNCTION(BlueprintCallable)
	float GetStrafeRange();

	UFUNCTION(BlueprintCallable)
	void OnEveryMontageEnded(UAnimMontage* montage, bool bInterrupted);  //모든 몽타주에 대해 검사함		

	//Navigation Invoker function
	FORCEINLINE class UNavigationInvokerComponent* GetNavInvoker() const { return navInvoker; };

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Trace")
	void PerformAttackTrace();

	//NavLink jump function
	UFUNCTION(BlueprintCallable)
	void JumpTo(FVector destination);

	//Patrol
	UFUNCTION(BlueprintCallable)
	void FindNearPatrolPoint();

	UFUNCTION()
	TArray<AActor*> GetPatrolPoint();

	UFUNCTION(BlueprintCallable, Category = "Enemy|Status")
	virtual void OnDeath() override;

	UFUNCTION(BlueprintCallable)
	void InitMonsterData();

public:
	// 애니메이션 실행 함수
	void PlayAttackAnim();
	void PlaySpawnAnim();

	void UpdateEnemyHealthBarRotation();				// 몬스터 HP바 회전 함수
	void UpdateOverheadEnemyHP(float const damage);		// 몬스터 HP 업데이트 함수

protected:
	// 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> SpawnMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 skillActionIdx;//어떤 스킬을 시전하는 중인지 저장, PerformActionTrace 에서 해당 변수를 참조해 행동하도록 함

	bool bIsPlayingAnim;

	// 오버헤드 위젯 관련
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|UI")
	UWidgetComponent* OverheadWidget;

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

	// 공격 트레이스 디버그 박스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DrawDebugLineSeconds;		 // 디버그 라인 나타낼 시간

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DrawDebugLineThickness;	 // 디버그 라인의 두께

	FTimerHandle detectDelayTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> projectile;//원거리 투사체

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ActionData")
	float meleeAtkRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ActionData")
	float strafeRange;

	// 데이터 테이블 관련
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ActionData")
	TArray<FMonsterAttackSkillData> MonsterAttackSkills;	// 몬스터 공격 스킬을 모아놓은 구조체를 배열로 저장
	TArray<FMonsterAttackTraceData> CachedAttackTraceDataArray;	// 공격 트레이스를 캐싱해두고 다른 스킬 사용시 인덱스에서 꺼내 쓰는 용도

public:
	static TArray<ARSDunMonsterCharacter*> AllMonsters;

	static const TArray<ARSDunMonsterCharacter*>& GetAllMonsters();

private:
	TObjectPtr<ARSMonsterAIController> AIController;  // TODO : 혹시나 캐싱해서 쓸 일 생길까봐 미리 만들어둠.

	const FMonsterData* GetFMonsterData();	// 코드 중복 부분 정리용 함수

	FTimerHandle MonsterHPBarRotationTimer;

// 사망시 아이템 드롭
private:
	UFUNCTION()
	void MonsterItemDrop();

};
