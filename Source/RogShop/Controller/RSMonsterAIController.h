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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RVO")//RVO radius
	float avoidanceRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RVO")//RVO priority
	float avoidanceWeight;

	int32 currentPatrolIdx;
public://Function
	ARSMonsterAIController();

	UFUNCTION(BlueprintCallable)
	void SetRVOAvoidanceEnabled(bool bEnable);

	//Behavior
	UFUNCTION(BlueprintCallable)
	void AIAttack();

	UFUNCTION(BlueprintCallable)
	void AISkill_1();

	UFUNCTION(BlueprintCallable)
	void AISkill_2();

	UFUNCTION(BlueprintCallable)
	void AISkill_3();

	//Patrol
	UFUNCTION(BlueprintCallable)
	void MoveToCurrentPatrolPoint();
};
