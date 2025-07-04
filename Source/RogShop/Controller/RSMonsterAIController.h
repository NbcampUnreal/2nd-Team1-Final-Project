// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RSDunMonsterCharacter.h"
#include "../Character/RSDunPlayerCharacter.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "RSMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSMonsterAIController : public AAIController
{
	GENERATED_BODY()
protected://Value	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RVO")//RVO radius
	float avoidanceRadius;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RVO")//RVO priority
	float avoidanceWeight;

	int32 currentPatrolIdx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float rotateSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float chaseLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float moveDistance;

	bool bIsChase;
	FVector priviousPos;
	FVector curPos;
public://Function
	ARSMonsterAIController();

	UFUNCTION(BlueprintCallable)
	void SetRVOAvoidanceEnabled(bool bEnable);

	//Behavior

	UFUNCTION(BlueprintCallable)
	void AISkillAction(int32 actionIdx, FVector interestedPos);

	UFUNCTION(BlueprintCallable)
	void CtrlPlaySpawnAnim();

	UFUNCTION(BlueprintCallable)
	bool IsMeleeSkill(int32 actionIdx);

	UFUNCTION(BlueprintCallable)
	bool IsFocusing(FVector lookFor);

	UFUNCTION(BlueprintCallable)
	void RotateToFocus(FVector lookFor, float deltaSecond);

	UFUNCTION(BlueprintCallable)
	void CalculateMoveLength();

	UFUNCTION(BlueprintCallable)
	void SetIsChase(bool bIsValid);

	UFUNCTION(BlueprintCallable)
	bool IsStillChase();

	UFUNCTION(BlueprintCallable)
	void SetPriviousPos();

	//Patrol
	UFUNCTION(BlueprintCallable)
	void MoveToCurrentPatrolPoint();
};
