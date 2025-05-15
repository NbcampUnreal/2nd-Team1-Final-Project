// Fill out your copyright notice in the Description page of Project Settings.


#include "RSCookingTile.h"

#include "RSTycoonInventoryComponent.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Food/RSBaseFood.h"
#include "RogShop/DataTable/CookFoodData.h"
#include "RogShop/GameInstanceSubsystem/RSDataSubsystem.h"
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
		RS_LOG_C("오더가 없습니다", FColor::Red)
		return;
	}

	//임시로 제일 가까이 있는거부터 제작하게
	FName FoodName = Orders[0];

	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->RemoveOrder(FoodName);
	Cook(FoodName);
}

void ARSCookingTile::Cook(const FName& FoodKey)
{
	State = ECookingState::Cooking;

	FCookFoodData* Data = GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Food
	                                       ->FindRow<FCookFoodData>(FoodKey, TEXT("Get FoodData"));

	//사용한 재료 제거
	for (auto& Need : Data->NeedIngredients)
	{
		GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->Inventory->Remove(Need.Key, Need.Value);
	}


	
	//임시, 바로 제작됨
	State = ECookingState::Finish;

	// FCookFoodData const* Data = GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Food->
	//                                                FindRow<FCookFoodData>(FoodKey, TEXT("Find Cook Data"));
	ARSBaseFood* Food = GetWorld()->SpawnActor<ARSBaseFood>(Data->ActorType);
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

		State = ECookingState::None;
	}
	else
	{
		RS_LOG_C("화구에 완성된 음식이 없습니다", FColor::Red)
	}
}
