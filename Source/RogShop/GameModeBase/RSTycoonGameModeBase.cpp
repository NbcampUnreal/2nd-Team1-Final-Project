// Fill out your copyright notice in the Description page of Project Settings.


#include "RogShop/GameModeBase//RSTycoonGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Tile/RSDoorTile.h"
#include "Tycoon/RSTycoonCustomerCharacter.h"


void ARSTycoonGameModeBase::StartGame()
{
	//임시
	CreateCustomer();
}

void ARSTycoonGameModeBase::AddOrder(const FString& OrderFoodName)
{
	OrderedFoodNames.Add(OrderFoodName);
}

// void ARSTycoonGameModeBase::AddCookedFood(const FString& CookedFoodName)
// {
// 	int32 OrderIndex = OrderedFoodNames.IndexOfByKey(CookedFoodName);
// 	if (OrderIndex == INDEX_NONE)
// 	{
// 		RS_LOG_C("주문 들어온 음식에 해당하는 음식이 없습니다", FColor::Red)
// 		return;
// 	}
//
// 	OrderedFoodNames.RemoveAt(OrderIndex);
// 	CookedFoods.Add(CookedFoodName);
// }

void ARSTycoonGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//임시
	GetWorldTimerManager().SetTimerForNextTick([&]()
	{
		StartGame();
	});
}

void ARSTycoonGameModeBase::CreateCustomer()
{
	NowCustomerCount++;

	//문이 한개라는 가정
	if (ARSDoorTile* DoorTile = Cast<ARSDoorTile>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSDoorTile::StaticClass())))
	{
		DoorTile->SpawnCustomer();
	}
	else
	{
		RS_LOG("DoorTile을 찾을 수 없습니다")
	}
}
