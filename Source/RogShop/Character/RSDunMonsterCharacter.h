// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunBaseCharacter.h"
#include "NavigationInvokerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RSDunMonsterCharacter.generated.h"

class ARSMonsterAIController;
class UMeleeAttackBoxComponent;

UCLASS()
class ROGSHOP_API ARSDunMonsterCharacter : public ARSDunBaseCharacter
{
	GENERATED_BODY()

public:
	ARSDunMonsterCharacter();

//	virtual void BeginPlay()override;

	// 애니메이션 실행 함수
	virtual void PlayBaseAttackAnim();
	virtual void PlayHitReactAnim();
	virtual void PlayDeathAnim();

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UMeleeAttackBoxComponent> MeleeAttackBoxComponent;

	//Navigation Invoker function
	FORCEINLINE class UNavigationInvokerComponent* GetNavInvoker() const { return navInvoker; };

	//NavLink jump function
	UFUNCTION(BlueprintCallable)
	void JumpTo(FVector destination);

protected:
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
private:
	TObjectPtr<ARSMonsterAIController> AIController;  // TODO : 혹시나 캐싱해서 쓸 일 생길까봐 미리 만들어둠.

};
