// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_RSCanTakeOrder.h"

#include "AIController.h"
#include "RSTycoonGameModeBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "RogShop/UtilDefine.h"
#include "Tycoon/NPC/RSTycoonCustomerCharacter.h"

bool UBTD_RSCanTakeOrder::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	const FName FindValueKey = TEXT("TargetCustomer");
	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();

	if (BlackBoard->GetValueAsObject(FindValueKey))
	{
		return false;
	}
	
	ARSTycoonGameModeBase* GameMode =  GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)
	
	auto& Customers = GameMode->GetCustomers();
	for (auto& CustomerPtr : Customers)
	{
		ARSTycoonCustomerCharacter* Customer = CustomerPtr.Get();
		check(Customer)

		if (Customer->GetState() == ETycoonCustomerState::OrderWaiting)
		{
			BlackBoard->SetValueAsObject(FindValueKey, Customer);
			return true;
		}
	}
	
	return false;
}
