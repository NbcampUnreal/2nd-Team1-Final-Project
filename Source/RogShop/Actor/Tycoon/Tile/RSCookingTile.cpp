// Fill out your copyright notice in the Description page of Project Settings.


#include "RSCookingTile.h"

#include "RSTycoonInventoryComponent.h"
#include "RSTycoonPlayerController.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Food/RSBaseFood.h"
#include "RogShop/DataTable/CookFoodData.h"
#include "RogShop/GameInstanceSubsystem/RSDataSubsystem.h"
#include "RogShop/GameModeBase/RSTycoonGameModeBase.h"
#include "Tycoon/RSTycoonPlayerCharacter.h"
#include "Tycoon/NPC/RSTycoonWaiterCharacter.h"

ARSCookingTile::ARSCookingTile()
{
	TileDisplayName = TEXT("화구");

	FoodLocation = CreateDefaultSubobject<USceneComponent>(TEXT("FoodLocation"));
	FoodLocation->SetupAttachment(RootComponent);

	ChefLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ChefLocation"));
	ChefLocation->SetupAttachment(RootComponent);
}

void ARSCookingTile::Interact(ACharacter* InteractCharacter)
{
	Super::Interact(InteractCharacter);

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
		TakeFood(InteractCharacter);
	}
	else if (State == ECookingState::Cooking)
	{
		RS_LOG_F_C("요리중 입니다 : %s", FColor::Orange, *CookingFoodOrder.FoodKey.ToString())
	}
}

void ARSCookingTile::OrderToCook()
{
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)

	auto& Orders = GameMode->GetOrders();
	if (Orders.Num() == 0)
	{
		RS_LOG_C("오더가 없습니다", FColor::Red)
		return;
	}

	//임시로 제일 가까이 있는거부터 제작하게
	FFoodOrder FoodOrder = Orders[0];

	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->RemoveOrder(FoodOrder);
	Cook(FoodOrder);
}

void ARSCookingTile::Cook(FFoodOrder Order)
{
	State = ECookingState::Cooking;
	CookingFoodOrder = Order;

	FCookFoodData* Data = GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Food
	                                       ->FindRow<FCookFoodData>(Order.FoodKey, TEXT("Get FoodData"));

	//사용한 재료 제거
	for (auto& Need : Data->NeedIngredients)
	{
		GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>()->GetComponentByClass<URSBaseInventoryComponent>()
		          ->RemoveItem(Need.Key, Need.Value);
	}

	//5초 후 완성
	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &ARSCookingTile::FinishCook, 5.f, false);
}

void ARSCookingTile::FinishCook()
{
	State = ECookingState::Finish;

	FCookFoodData const* Data = GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Food->
	                                               FindRow<FCookFoodData>(CookingFoodOrder.FoodKey, TEXT("Find Cook Data"));

	ARSBaseFood* Food = GetWorld()->SpawnActor<ARSBaseFood>(Data->ActorType);
	Food->SetActorLocation(FoodLocation->GetComponentLocation());
	Food->WantCustomer = CookingFoodOrder.Customer;

	//오더 제거

	CookingFoodOrder.FoodKey = FName();
	CookingFoodOrder.Customer = nullptr;

	CookedFood = Food;
}

void ARSCookingTile::TakeFood(ACharacter* InteractCharacter)
{
	if (CookedFood)
	{
		IRSCanPickup* CanPickupCharacter = Cast<IRSCanPickup>(InteractCharacter);
		check(CanPickupCharacter)

		CanPickupCharacter->Pickup(CookedFood);

		CookedFood = nullptr;
		State = ECookingState::None;
	}
	else
	{
		RS_LOG_C("화구에 완성된 음식이 없습니다", FColor::Red)
	}
}
