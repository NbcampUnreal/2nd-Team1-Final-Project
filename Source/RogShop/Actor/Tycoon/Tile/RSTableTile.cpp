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

	if (SittingCustomers.Num() == 0)
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
		RS_LOG_C("잘못된 State", FColor::Red)
	}
}

bool ARSTableTile::Sit(const TArray<ARSTycoonCustomerCharacter*>& Customers)
{
	int32 MaxPlace = SittingLocations.Num();
	if (Customers.Num() > MaxPlace)
	{
		return false;
	}

	RS_LOG("손님이 앉음")
	SittingCustomers = Customers;

	for (int32 i = 0; i < Customers.Num(); i++)
	{
		Customers[i]->SetActorLocation(SittingLocations[i]->GetComponentLocation());
		Customers[i]->State = ETycoonCustomerState::OrderWaiting;
	}

	return true;
}

FVector ARSTableTile::GetFoodLocation() const
{
	return FoodLocation->GetComponentLocation();
}

void ARSTableTile::Order()
{
	RS_LOG("주문을 받음")

	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	for (auto& Customer : SittingCustomers)
	{
		GameMode->AddOrder(Customer->WantFoodName);
		Customer->State = ETycoonCustomerState::FoodWaiting;
	}
}

void ARSTableTile::Serving()
{
	ARSTycoonPlayerCharacter* Player = Cast<ARSTycoonPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	check(Player)

	RS_LOG("음식을 전달함")
	Player->Drop(FoodLocation->GetComponentLocation());

	for (auto& Customer : SittingCustomers)
	{
		Customer->State = ETycoonCustomerState::Eat;
		RS_LOG("손님이 음식을 먹어욧")
	}
}
