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