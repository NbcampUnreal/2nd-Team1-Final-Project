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

	//손님이 앉아있고 주문을 신청하면 주문 받기
	//음식을 들고 있다면 요리를 완성한 음식을 FoodLocation에 배치
	// ㄴ 배치 후 조금이따가 돈 벌리면서 손님이 사라짐
	
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

void ARSTableTile::Sit(const TArray<ARSTycoonCustomerCharacter*>& Customers)
{
	if (Customers.Num() > GetMaxPlace())
	{
		RS_LOG_C("손님이 좌석수보다 많습니다", FColor::Red)
		return;
	}

	RS_LOG("손님이 앉음")
	SittingCustomers = Customers;
	
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	for (int32 i = 0; i < Customers.Num(); i++)
	{
		Customers[i]->SetActorLocation(SittingLocations[i]->GetComponentLocation());
		Customers[i]->State = ETycoonCustomerState::OrderWaiting;

		//오더 자체는 시스템적으로 들어가있음, 손님한테 가서 상호작용을 해서 음식을 기다리는 상태로 만들어야하는거임 
		GameMode->AddOrder(Customers[i]->WantFoodKey);
	}
}

FVector ARSTableTile::GetFoodPosition() const
{
	return FoodLocation->GetComponentLocation();
}

void ARSTableTile::Order()
{
	RS_LOG("주문을 받음")

	for (auto& Customer : SittingCustomers)
	{
		Customer->State = ETycoonCustomerState::FoodWaiting;
	}
}

void ARSTableTile::Serving()
{
	ARSTycoonPlayerCharacter* Player = Cast<ARSTycoonPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	check(Player)

	bool bComplete = Player->Drop(FoodLocation->GetComponentLocation());
	if (bComplete)
	{
		RS_LOG("음식을 전달했습니다")
		for (auto& Customer : SittingCustomers)
		{
			Customer->State = ETycoonCustomerState::Eat;
		}
	}
}
