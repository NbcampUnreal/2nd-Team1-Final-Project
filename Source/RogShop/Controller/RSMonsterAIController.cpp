// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMonsterAIController.h"

ARSMonsterAIController::ARSMonsterAIController()
{
	avoidanceRadius = 0.0f;
	avoidanceWeight = 0.0f;
/*	NavGenerationRadius = 100.0f;
	NavRemovalRadius = 150.0f;
	NavInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));
	NavInvoker->SetGenerationRadii(NavGenerationRadius, NavRemovalRadius);*/
}
/*
void ARSMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!InPawn)
	{
		return;
	}

	UNavigationInvokerComponent* extInvoker = InPawn->FindComponentByClass<UNavigationInvokerComponent>();
	if (!extInvoker)
	{
		UNavigationInvokerComponent* invokerComp = NewObject<UNavigationInvokerComponent>(InPawn);
		if (invokerComp)
		{
			invokerComp->RegisterComponent(); // 필수!
			InPawn->AddInstanceComponent(invokerComp);

			// 기본값 설정 (필요시)
			invokerComp->SetGenerationRadius(3000.0f);
			invokerComp->SetRemovalRadius(5000.0f);
		}
	}
}*/

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