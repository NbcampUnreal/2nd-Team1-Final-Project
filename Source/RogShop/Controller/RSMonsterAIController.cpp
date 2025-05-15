// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMonsterAIController.h"

ARSMonsterAIController::ARSMonsterAIController()
{
	avoidanceRadius = 0.0f;
	avoidanceWeight = 0.0f;
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
			
		}
	}
}

void ARSMonsterAIController::MoveToCurrentPatrolPoint()
{
	APawn* ctrlPawn = GetPawn();
	if (ctrlPawn)
	{
		ARSDunMonsterCharacter* monster = Cast<ARSDunMonsterCharacter>(GetPawn());
		if (!monster)
		{
			return;
		}

		// ���� ����Ʈ�� �ϳ��� ���ٸ� �̵��� �ʿ� ����
		if (patrolPoints.Num() == 0)
		{
			return;
		}

		MoveToActor(
			patrolPoints[currentPatrolIdx],
			5.0f,   // AcceptanceRadius: ��ǥ ���� ��ó �� ���� �̳��� �����ϸ� ������
			true,   // bStopOnOverlap
			true,   // bUsePathfinding
			false,  // bCanStrafe: �⺻ �̵� ��忡�� �¿�� ȸ�� ���� �̵� ���� ����
			nullptr,// FilterClass: ��� ����. ����Ʈ ���
			true    // bAllowPartialPath: �κ� ��� ��� ����
		);

		currentPatrolIdx = (currentPatrolIdx + 1) % patrolPoints.Num();
	}
}
/*
void ARSMonsterAIController::FindNearPatrolPoint()
{
	APawn* ctrlPawn = GetPawn();
	if (ctrlPawn)
	{//regist detect area to pawn
		USphereComponent* detectSphere = NewObject<USphereComponent>(ctrlPawn);
		if (!detectSphere) return;

		detectSphere->RegisterComponent();
		detectSphere->AttachToComponent(ctrlPawn->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		detectSphere->SetSphereRadius(500.f);
		detectSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		detectSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		detectSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		detectSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
		detectSphere->SetGenerateOverlapEvents(true);

		//Overlap event bind
		detectSphere->OnComponentBeginOverlap.AddDynamic(this, &ARSDunMonsterCharacter:: );
	}
}*/

void ARSMonsterAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.Code == EPathFollowingResult::Success)
	{
		MoveToCurrentPatrolPoint();
	}
}