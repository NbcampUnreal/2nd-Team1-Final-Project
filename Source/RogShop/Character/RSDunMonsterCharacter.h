// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunBaseCharacter.h"
#include "NavigationInvokerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Engine/TargetPoint.h"
#include "Engine/OverlapResult.h"
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

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UMeleeAttackBoxComponent> MeleeAttackBoxComponent;

	//Navigation Invoker function
	FORCEINLINE class UNavigationInvokerComponent* GetNavInvoker() const { return navInvoker; };

	//NavLink jump function
	UFUNCTION(BlueprintCallable)
	void JumpTo(FVector destination);

	//Patrol
	UFUNCTION(BlueprintCallable)
	void FindNearPatrolPoint();

	UFUNCTION()
	TArray<AActor*> GetPatrolPoint();
protected:
	// 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* BaseAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* DeathMontage;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Patrol")
	USphereComponent* detectSphere;

	UPROPERTY(BlueprintReadWrite, Category = "Patrol")
	float maxDetectPatrolRoute;
private:
	TObjectPtr<ARSMonsterAIController> AIController;  // TODO : 혹시나 캐싱해서 쓸 일 생길까봐 미리 만들어둠.

};
