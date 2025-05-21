// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTableTile.h"

#include "RogShop/UtilDefine.h"
#include "RogShop/GameModeBase/RSTycoonGameModeBase.h"
#include "Tycoon/RSTycoonCustomerCharacter.h"
#include "Tycoon/RSTycoonPlayerCharacter.h"


ARSTableTile::ARSTableTile()
{
	TileName = TEXT("테이블");

	FoodLocation = CreateDefaultSubobject<USceneComponent>("FoodLocation");
	FoodLocation->SetupAttachment(RootComponent);
}

void ARSTableTile::Interact()
{
	Super::Interact();
	
	if (!Use())
	{
		return;
	}

	TWeakObjectPtr<ARSTycoonCustomerCharacter> MainCustomer = SittingCustomers[0];
	check(MainCustomer.IsValid())

	if (MainCustomer->State == ETycoonCustomerState::OrderWaiting)
	{
		//주문을 받음
		Order();
	}
	else if (MainCustomer->State == ETycoonCustomerState::FoodWaiting)
	{
		//음식을 전달함
		Serving();
	}
	else
	{
		RS_LOG_F_C("잘못된 접근, 현재 손님 State : %s", FColor::Red, *UEnum::GetValueAsString(MainCustomer->State))
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

	Customer->Sit(SittingLocations[CustomerIndex]->GetComponentTransform());
	Customer->OnLeave.AddLambda([&](ARSTycoonCustomerCharacter* LeaveCustomer)
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

void ARSTableTile::Serving()
{
	ARSTycoonPlayerCharacter* Player = Cast<ARSTycoonPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	check(Player)

	AActor* Food = Player->Drop(FoodLocation->GetComponentLocation());
	if (Food)
	{
		RS_LOG("음식을 전달했습니다")

		//임시, 지금 기획이 좀 바뀌면서 이상함.
		//수정해야함
		FoodActor = Food;
		
		for (auto& Customer : SittingCustomers)
		{
			Customer->Eat();
		}
	}
	else
	{
		RS_LOG_C("음식을 전달하지 못했습니다", FColor::Red)
	}
}
