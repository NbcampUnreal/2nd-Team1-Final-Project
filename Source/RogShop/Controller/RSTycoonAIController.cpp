// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonAIController.h"
#include "Perception/AIPerceptionTypes.h"

// ������ Ű �̸� ����
const FName ARSTycoonAIController::TargetActorKey = "TargetPlayer";

ARSTycoonAIController::ARSTycoonAIController()
{
	BlackboardComponent = nullptr;
}

void ARSTycoonAIController::OnPossess(APawn* InPawn)
{
}

void ARSTycoonAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
}
