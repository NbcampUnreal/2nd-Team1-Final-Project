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

	FoodLocation = CreateDefaultSubobject<USceneComponent>("FoodLocation");
	FoodLocation->SetupAttachment(RootComponent);
}

void ARSTableTile::Interact(ACharacter* InteractCharacter)
{
	Super::Interact(InteractCharacter);

	if (!Use())
	{
		return;
	}

	TWeakObjectPtr<ARSTycoonCustomerCharacter> MainCustomer = SittingCustomers[0];
	check(MainCustomer.IsValid())

	if (MainCustomer->GetState() == ETycoonCustomerState::OrderWaiting)
	{
		//주문을 받음
		Order();
	}
	else if (MainCustomer->GetState() == ETycoonCustomerState::FoodWaiting)
	{
		//음식을 전달함
		Serving(InteractCharacter);
	}
	else
	{
		RS_LOG_F_C("잘못된 접근, 현재 손님 State : %s", FColor::Red, *UEnum::GetValueAsString(MainCustomer->GetState()))
	}
}

void ARSTableTile::Sit(ARSTycoonCustomerCharacter* Customer)
{
	if (SittingCustomers.Num() >= GetMaxPlace())
	{
		RS_LOG_C("손님이 좌석수보다 많습니다", FColor::Red)
		return;
	}

	RS_LOG("손님이 앉음")

	int32 CustomerIndex = SittingCustomers.Num();
	SittingCustomers.Add(Customer);

	Customer->Sit(this, SittingLocations[CustomerIndex]->GetComponentTransform());
	Customer->OnFinishEat.AddLambda([&](ARSTycoonCustomerCharacter* LeaveCustomer)
	{
		SittingCustomers.RemoveSingle(LeaveCustomer);

		FoodActor->Destroy();
		FoodActor = nullptr;
	});
}

void ARSTableTile::Order()
{
	RS_LOG("주문을 받음")

	for (auto& Customer : SittingCustomers)
	{
		Customer->WaitFood();
	}
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

	ARSTycoonCustomerCharacter* OrderedCustomer = nullptr;
	for (auto Customer : SittingCustomers)
	{
		if (Customer == Food->Order.Customer)
		{
			OrderedCustomer = Customer;
			break;
		}
	}
	
	if (OrderedCustomer)
	{
		RS_LOG("음식을 전달했습니다")

		FoodActor = PickupActor;
		
		CanPickupCharacter->Drop(FoodLocation->GetComponentLocation());
		OrderedCustomer->Eat();
	}
	else
	{
		RS_LOG_C("주문하신 손님이 아닙니다.", FColor::Red)
	}
}
