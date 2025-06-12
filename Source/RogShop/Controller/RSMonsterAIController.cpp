// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMonsterAIController.h"

ARSMonsterAIController::ARSMonsterAIController()
{
	avoidanceRadius = 100.0f;
	avoidanceWeight = 100.0f;
	currentPatrolIdx = 0;

	rotateSpeed = 0.01f;
}

void ARSMonsterAIController::SetRVOAvoidanceEnabled(bool bEnable)//path find bottleneck resolving function
{
	APawn* ctrlPawn = GetPawn();//Figure out Who is possessed Pawn
	if (ctrlPawn)
	{
		ARSDunMonsterCharacter* ctrlChr;
		ctrlChr = Cast<ARSDunMonsterCharacter>(ctrlPawn);//Figure out who is possessed Character
		if (ctrlChr)
		{
			UCharacterMovementComponent* movementComponent = ctrlChr->GetCharacterMovement();
			//"GameFramework/CharacterMovementComponent.h" Needed. might be better existing at character class
			if (movementComponent)
			{
				movementComponent->bUseRVOAvoidance = bEnable;
				movementComponent->AvoidanceConsiderationRadius = avoidanceRadius;
				movementComponent->AvoidanceWeight = avoidanceWeight;
			}
		}
	}	
}

void ARSMonsterAIController::AISkillAction(int32 actionIdx, FVector interestedPos)
{
	APawn* ctrlPawn = GetPawn();
	if (ctrlPawn)
	{
		ARSDunMonsterCharacter* ctrlChr;
		ctrlChr = Cast<ARSDunMonsterCharacter>(ctrlPawn);//Figure out who is possessed Character
		if (ctrlChr&&!ctrlChr->GetIsPlayingAnim())
		{
			ctrlChr->PlayAction(actionIdx, interestedPos);
		}
	}
}

void ARSMonsterAIController::CtrlPlaySpawnAnim()
{
	APawn* ctrlPawn = GetPawn();
	if (ctrlPawn)
	{
		ARSDunMonsterCharacter* ctrlChr;
		ctrlChr = Cast<ARSDunMonsterCharacter>(ctrlPawn);//Figure out who is possessed Character
		if (ctrlChr)
		{
			ctrlChr->PlaySpawnAnim();
		}
	}
}

bool ARSMonsterAIController::IsMeleeSkill(int32 actionIdx)
{
	APawn* ctrlPawn = GetPawn();
	bool bIsMelee;
	if (ctrlPawn)
	{
		ARSDunMonsterCharacter* ctrlChr;
		ctrlChr = Cast<ARSDunMonsterCharacter>(ctrlPawn);//Figure out who is possessed Character
		if (ctrlChr)
		{
			bIsMelee = ctrlChr->GetIsMeleeSkill(actionIdx);
		}
	}
	return bIsMelee;
}

void ARSMonsterAIController::MoveToCurrentPatrolPoint()
{
	APawn* ctrlPawn = GetPawn();
	
	if (ctrlPawn)
	{
		ARSDunMonsterCharacter* monster = Cast<ARSDunMonsterCharacter>(ctrlPawn);
		if (!monster)
		{
			return;
		}
		TArray<AActor*> patrolPoints = monster->GetPatrolPoint();
		// 순찰 포인트가 하나도 없다면 이동할 필요 없음
		if (patrolPoints.Num() == 0)
		{
			return;
		}

		MoveToActor(
			patrolPoints[currentPatrolIdx],
			5.0f,   // AcceptanceRadius: 목표 지점 근처 몇 유닛 이내에 도달하면 멈출지
			true,   // bStopOnOverlap
			true,   // bUsePathfinding
			false,  // bCanStrafe: 기본 이동 모드에서 좌우로 회전 없이 이동 가능 여부
			nullptr,// FilterClass: 경로 필터. 디폴트 사용
			true    // bAllowPartialPath: 부분 경로 허용 여부
		);

		currentPatrolIdx = (currentPatrolIdx + 1) % patrolPoints.Num();
	}
}

bool ARSMonsterAIController::IsFocusing(FVector lookFor)
{
	APawn* ctrlPawn = GetPawn();
	FVector look;
	FVector direction;
	float dot;
	bool bIsLook = true;
	if (ctrlPawn)
	{
		look = ctrlPawn->GetActorForwardVector();
		direction = (lookFor - ctrlPawn->GetActorLocation()).GetSafeNormal();
		dot = FVector::DotProduct(look, direction);

		if (dot >= 0.99f)
		{
			bIsLook = true;
		}
		else
		{
			bIsLook = false;
		}
	}

	return bIsLook;
}

void ARSMonsterAIController::RotateToFocus(FVector lookFor)
{
	APawn* ctrlPawn = GetPawn();
	FVector chrPos;
	FVector look;
	FRotator chrRot;
	FRotator newRot;
	float targetYaw;
	float currentYaw;
	float deltaYaw;

	if (ctrlPawn)
	{
		chrPos = ctrlPawn->GetActorLocation();
		chrRot = ctrlPawn->GetActorRotation();
		look = (lookFor - chrPos).GetSafeNormal();

		targetYaw = look.Rotation().Yaw;
		currentYaw = chrRot.Yaw;
		deltaYaw = FMath::FindDeltaAngleDegrees(currentYaw, targetYaw);

		newRot = chrRot + FRotator(0, deltaYaw*rotateSpeed, 0);

		ctrlPawn->SetActorRotation(newRot);
	}
	else
	{
		return;
	}
}