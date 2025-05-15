// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsAtkRange.h"
#include "../Controller/RSMonsterAIController.h"
#include "../Character/RSDunMonsterCharacter.h"
#include "../Character/RSDunPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

const float UBTD_IsAtkRange::atkRange(200.f);

UBTD_IsAtkRange::UBTD_IsAtkRange()
{
	NodeName = TEXT("IsAtkRange");
}

bool UBTD_IsAtkRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	checkf(bResult == true, TEXT("Super::CalculateRawConditionValue() function has returned false."));

	ARSMonsterAIController* AICon = Cast<ARSMonsterAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AICon) == true, TEXT("Invalid AIController."));

	ARSDunMonsterCharacter* NPC = Cast<ARSDunMonsterCharacter>(AICon->GetPawn());
	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));

/*	ARSDunPlayerCharacter* targetPlayer = Cast<ARSDunPlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ARSMonsterAIController::Target));
	if(IsValid(targetPlayer)&&targetPlayer->IsPlayerControlled())
	{
		return NPC->GetDistanceTo(targetPlayer) <= atkRange;
	}*/

	return false;
}