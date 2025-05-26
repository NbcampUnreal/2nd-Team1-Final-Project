// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_RSCanTakeOrder.h"

#include "AIController.h"
#include "RSTycoonGameModeBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "RogShop/UtilDefine.h"
#include "Tycoon/NPC/RSTycoonCustomerCharacter.h"
#include "Tycoon/NPC/RSTycoonWaiterCharacter.h"

bool UBTD_RSCanTakeOrder::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	const static FName TargetCustomerKey = TEXT("TargetCustomer");
	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();

	if (BlackBoard->GetValueAsObject(TargetCustomerKey))
	{
		return false;
	}

	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)

	auto& Customers = GameMode->GetCustomers();
	for (auto& CustomerPtr : Customers)
	{
		ARSTycoonCustomerCharacter* Customer = CustomerPtr.Get();
		check(Customer)

		if (Customer->GetState() == ETycoonCustomerState::OrderWaiting)
		{
			//다른 웨이터에게 이 손님이 할당 되어 있다면 제외
			if (CheckAssignOtherWaiter(TargetCustomerKey, Customer))
			{
				continue;
			}

			BlackBoard->SetValueAsObject(TargetCustomerKey, Customer);
			return true;
		}
	}

	return false;
}

bool UBTD_RSCanTakeOrder::CheckAssignOtherWaiter(const FName& Key, ARSTycoonCustomerCharacter* Target) const
{
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	for (ARSTycoonNPC* NPC : GameMode->GetNPCs())
	{
		if (ARSTycoonWaiterCharacter* Waiter = Cast<ARSTycoonWaiterCharacter>(NPC))
		{
			AAIController* WaiterController = Cast<AAIController>(Waiter->GetController());
			ARSTycoonCustomerCharacter* OtherWaiterTargetCustomer = Cast<ARSTycoonCustomerCharacter>(
				WaiterController->GetBlackboardComponent()->GetValueAsObject(Key));

			if (OtherWaiterTargetCustomer == Target)
			{
				return true;
			}
		}
	}

	return false;
}
