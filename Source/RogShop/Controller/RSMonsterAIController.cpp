// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMonsterAIController.h"

ARSMonsterAIController::ARSMonsterAIController()
{
	avoidanceRadius = 0.0f;
	avoidanceWeight = 0.0f;
	currentPatrolIdx = 0;
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

void ARSMonsterAIController::AIAttack()
{
	APawn* ctrlPawn = GetPawn();
	if (ctrlPawn)
	{
		ARSDunMonsterCharacter* ctrlChr;
		ctrlChr = Cast<ARSDunMonsterCharacter>(ctrlPawn);//Figure out who is possessed Character
		if (ctrlChr)
		{
			ctrlChr->PlayBaseAttackAnim();
		}
	}
}

void ARSMonsterAIController::AISkill_1()
{
	APawn* ctrlPawn = GetPawn();
	if (ctrlPawn)
	{
		ARSDunMonsterCharacter* ctrlChr;
		ctrlChr = Cast<ARSDunMonsterCharacter>(ctrlPawn);//Figure out who is possessed Character
		if (ctrlChr)
		{
			ctrlChr->PlaySkill_1();
		}
	}
}

void ARSMonsterAIController::AISkill_2()
{
	APawn* ctrlPawn = GetPawn();
	if (ctrlPawn)
	{
		ARSDunMonsterCharacter* ctrlChr;
		ctrlChr = Cast<ARSDunMonsterCharacter>(ctrlPawn);//Figure out who is possessed Character
		if (ctrlChr)
		{
			ctrlChr->PlaySkill_2();
		}
	}
}

void ARSMonsterAIController::AISkill_3()
{
	APawn* ctrlPawn = GetPawn();
	if (ctrlPawn)
	{
		ARSDunMonsterCharacter* ctrlChr;
		ctrlChr = Cast<ARSDunMonsterCharacter>(ctrlPawn);//Figure out who is possessed Character
		if (ctrlChr)
		{
			ctrlChr->PlaySkill_3();
		}
	}
}

void ARSMonsterAIController::AISkill_4()
{
	APawn* ctrlPawn = GetPawn();
	if (ctrlPawn)
	{
		ARSDunMonsterCharacter* ctrlChr;
		ctrlChr = Cast<ARSDunMonsterCharacter>(ctrlPawn);//Figure out who is possessed Character
		if (ctrlChr)
		{
			ctrlChr->PlaySkill_4();
		}
	}
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
