// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTableTile.h"

#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Food/RSBaseFood.h"
#include "RogShop/GameModeBase/RSTycoonGameModeBase.h"
#include "Tycoon/NPC/RSTycoonCustomerCharacter.h"
#include "Tycoon/RSTycoonPlayerCharacter.h"
#include "Tycoon/NPC/RSTycoonWaiterCharacter.h"


ARSTableTile::ARSTableTile()
{
	TileDisplayName = TEXT("테이블");
}

void ARSTableTile::Interact(ACharacter* InteractCharacter)
{
	Super::Interact(InteractCharacter);

	int32 InteractIndex = GetOrderWaitCustomerIndex();
	if (InteractIndex != INDEX_NONE)
	{
		//주문을 받음
		Order(SittingCustomers[InteractIndex]);
	}
	else if (InteractCharacter->Implements<URSCanPickup>())
	{
		//음식을 전달함
		Serving(InteractCharacter);
	}
	else
	{
		RS_LOG_C("상호작용이 불가능합니다.", FColor::Red)
	}
}

void ARSTableTile::Sit(ARSTycoonCustomerCharacter* Customer)
{
	int32 CanSitIndex = GetCanSitingLocationIndex();
	if (CanSitIndex == INDEX_NONE)
	{
		RS_LOG_C("테이블에 앉을 수 없습니다", FColor::Red)
		return;
	}
	
	RS_LOG("손님이 앉음")
 
	SittingCustomers[CanSitIndex] = Customer;

	Customer->Sit(this, SittingLocations[CanSitIndex]->GetComponentTransform());
	Customer->OnFinishEat.AddLambda([&](ARSTycoonCustomerCharacter* LeaveCustomer)
	{
		int32 Index = INDEX_NONE;
		for (int32 i = 0; i < SittingCustomers.Num(); i++)
		{
			if (SittingCustomers[i] == LeaveCustomer)
			{
				Index = i;
			}
		}
		
		SittingCustomers[Index] = nullptr;

		FoodActors[Index]->Destroy();
		FoodActors[Index] = nullptr;
	});
}

int32 ARSTableTile::GetOrderWaitCustomerIndex()
{
	for (int i = 0; i < SittingCustomers.Num(); i++)
	{
		if (SittingCustomers[i] != nullptr &&
			SittingCustomers[i]->GetState() == ETycoonCustomerState::OrderWaiting)
		{
			return i;
		}
	}

	return INDEX_NONE;
}

int32 ARSTableTile::GetFoodWaitCustomerIndex(FFoodOrder Order)
{
	for (int i = 0; i < SittingCustomers.Num(); i++)
	{
		if (SittingCustomers[i] != nullptr &&
			SittingCustomers[i]->GetState() == ETycoonCustomerState::FoodWaiting &&
			SittingCustomers[i]->WantFoodKey == Order.FoodKey &&
			SittingCustomers[i] == Order.Customer)
		{
			return i;
		}
	}

	return INDEX_NONE;
}


int32 ARSTableTile::GetCanSitingLocationIndex() const
{
	for (int i = 0; i < SittingCustomers.Num(); i++)
	{
		if (SittingCustomers[i] == nullptr)
		{
			return i;
		}
	}

	return INDEX_NONE;
}

bool ARSTableTile::CanSit() const
{
	return GetCanSitingLocationIndex() != INDEX_NONE;
}

void ARSTableTile::BeginPlay()
{
	Super::BeginPlay();

	FoodActors.SetNum(FoodLocations.Num());
	SittingCustomers.SetNum(SittingLocations.Num());
}

void ARSTableTile::Order(ARSTycoonCustomerCharacter* Customer)
{
	RS_LOG("주문을 받음")

	Customer->WaitFood();
}

void ARSTableTile::Serving(ACharacter* InteractCharacter)
{
	IRSCanPickup* CanPickupCharacter = Cast<IRSCanPickup>(InteractCharacter);
	check(CanPickupCharacter)

	AActor* PickupActor = CanPickupCharacter->GetPickupActor();
	if (PickupActor == nullptr)
	{
		RS_LOG_C("들고있는 음식이 없습니다", FColor::Red)
		return;
	}

	ARSBaseFood* Food = Cast<ARSBaseFood>(PickupActor);
	if (Food == nullptr)
	{
		RS_LOG_C("들고 있는 액터가 음식이 아닙니다.", FColor::Red)
		return;
	}

	int ServingCustomerIndex = INDEX_NONE;
	for (int i = 0; i < SittingCustomers.Num(); i++)
	{
		if (SittingCustomers[i] == Food->Order.Customer)
		{
			ServingCustomerIndex = i;
			break;
		}
	}

	ARSTycoonCustomerCharacter* OrderedCustomer = SittingCustomers[ServingCustomerIndex];
	if (OrderedCustomer)
	{
		RS_LOG("음식을 전달했습니다")

		FoodActors[ServingCustomerIndex] = PickupActor;

		CanPickupCharacter->Drop(FoodLocations[ServingCustomerIndex]->GetComponentLocation());
		OrderedCustomer->Eat();
	}
	else
	{
		RS_LOG_C("주문하신 손님이 없습니다.", FColor::Red)
	}
}