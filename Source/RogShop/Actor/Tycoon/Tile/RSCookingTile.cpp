// Fill out your copyright notice in the Description page of Project Settings.


#include "RSCookingTile.h"

#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Food/RSBaseFood.h"
#include "RogShop/GameModeBase/RSTycoonGameModeBase.h"
#include "Tycoon/RSTycoonPlayerCharacter.h"


ARSCookingTile::ARSCookingTile()
{
	TileName = TEXT("화구");

	FoodLocation = CreateDefaultSubobject<USceneComponent>(TEXT("FoodLocation"));
	FoodLocation->SetupAttachment(RootComponent);
}

void ARSCookingTile::Interact()
{
	Super::Interact();

	//주문이 들어온 요리를 제작할 수 있음
	//일단 0번째를 자동으로 요리하는식으로 제작

	if (State == ECookingState::None)
	{
		//요리하라고 명령 내림
		OrderToCook();
	}
	else if (State == ECookingState::Finish)
	{
		//음식 가져감
		TakeFood();
	}
}

void ARSCookingTile::OrderToCook()
{
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)

	auto& Orders = GameMode->GetOrders();
	if (Orders.Num() == 0)
	{
		return;
	}

	//임시로 제일 가까이 있는거부터 제작하게
	Cook(Orders[0]);
}

void ARSCookingTile::Cook(const FString& FoodName)
{
	State = ECookingState::Cooking;

	//임시, 바로 제작됨
	//임시, 음식의 형태를 임의로 한가지로 고정함. 데이터 테이블에서 종류에 따라 생성되는 모델링을 바꿀 예정
	State = ECookingState::Finish;

	ARSBaseFood* Food = GetWorld()->SpawnActor<ARSBaseFood>(FoodType);
	Food->SetActorLocation(FoodLocation->GetComponentLocation());

	CookedFood = Food;
}

void ARSCookingTile::TakeFood()
{
	ARSTycoonPlayerCharacter* Player = Cast<ARSTycoonPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	check(Player)

	if (ARSBaseFood* Food = CookedFood.Get())
	{
		Player->Pickup(Food);
		CookedFood = nullptr;
	}
	else
	{
		RS_LOG_C("화구에 완성된 음식이 없습니다", FColor::Red)
	}
}
