// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMonsterAIController.h"

ARSMonsterAIController::ARSMonsterAIController()
{
	avoidanceRadius = 500.0f;
	avoidanceWeight = 500.0f;
	currentPatrolIdx = 0;

	rotateSpeed = 0.01f;
	chaseLength = 20000.f;
	moveDistance = 0;
	bIsChase = false;
	priviousPos = FVector(0.f,0.f,0.f);
	curPos = FVector(0.f, 0.f, 0.f);
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
	FRotator lookRot;
	FRotator dirRot;
	FQuat lookQuat;
	FQuat dirQuat;
	FQuat deltaQuat;
	float dot;
	bool bIsLook = true;
	if (ctrlPawn)
	{
		look = ctrlPawn->GetActorForwardVector();
		direction = (lookFor - ctrlPawn->GetActorLocation()).GetSafeNormal();
//		dot = FVector::DotProduct(look, direction);

//		DrawDebugLine(GetWorld(), ctrlPawn->GetActorLocation(), look, FColor::Red, false, 5.0f, 0, 2.0f);
//		DrawDebugLine(GetWorld(), ctrlPawn->GetActorLocation(), direction, FColor::Blue, false, 5.0f, 0, 2.0f);

		lookRot = look.Rotation();
		dirRot = direction.Rotation();
		lookQuat = FQuat(lookRot);
		dirQuat = FQuat(dirRot);

		dot = lookQuat | dirQuat;
		deltaQuat = lookQuat * dirQuat.Inverse();
//		dot = deltaQuat.GetAngle();

		if (dot >= 0.95f)
//		if(dot<10.f)
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

void ARSMonsterAIController::RotateToFocus(FVector lookFor, float deltaSecond)
{
	APawn* ctrlPawn = GetPawn();
	FVector chrPos;
	FVector look;
	FRotator chrRot;
	FRotator lookRot;
	FRotator newRot;
	float targetYaw;
	float currentYaw;
	float deltaYaw;

	if (ctrlPawn)
	{
		chrPos = ctrlPawn->GetActorLocation();
		chrRot = ctrlPawn->GetActorRotation();
		look = (lookFor - chrPos).GetSafeNormal();
		lookRot = look.Rotation();

		chrRot.Pitch = 0.f;
		chrRot.Roll = 0.f;
		lookRot.Pitch = 0.f;
		lookRot.Roll = 0.f;

		targetYaw = look.Rotation().Yaw;
		currentYaw = chrRot.Yaw;
		deltaYaw = FMath::Clamp(FMath::FindDeltaAngleDegrees(currentYaw, targetYaw),0.0f,1.0f);

//		DrawDebugLine(GetWorld(), ctrlPawn->GetActorLocation(), look, FColor::Blue, false, 5.0f, 0, 2.0f);
//		DrawDebugLine(GetWorld(), ctrlPawn->GetActorLocation(), ctrlPawn->GetActorForwardVector(), FColor::Red, false, 5.0f, 0, 2.0f);

//		newRot = FMath::RInterpTo(chrRot, look.Rotation(), deltaSecond * 10.f, 0.3f);
		newRot = FMath::RInterpTo(chrRot, lookRot, deltaSecond * 10.f, 0.3f);

		ctrlPawn->SetActorRotation(newRot);
	}
	else
	{
		return;
	}
}

void ARSMonsterAIController::CalculateMoveLength()
{
	if (bIsChase)
	{
		APawn* ctrlPawn = GetPawn();
		if (ctrlPawn)
		{
			curPos = ctrlPawn->GetActorLocation();
			moveDistance += FVector::Dist(curPos, priviousPos);
		}
	}
}

void ARSMonsterAIController::SetIsChase(bool bIsValid)
{
	bIsChase = bIsValid;
}

void ARSMonsterAIController::SetPriviousPos()
{
	APawn* ctrlPawn = GetPawn();
	if (ctrlPawn)
	{
		priviousPos = ctrlPawn->GetActorLocation();
	}
}

bool ARSMonsterAIController::IsStillChase()
{
	return chaseLength < moveDistance ? false : true;
}