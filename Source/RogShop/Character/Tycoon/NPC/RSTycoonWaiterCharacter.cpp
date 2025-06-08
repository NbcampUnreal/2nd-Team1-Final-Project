// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonWaiterCharacter.h"

#include "AIController.h"
#include "RSTycoonCustomerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "RogShop/Actor/Tycoon/Food/RSBaseFood.h"
#include "RogShop/Actor/Tycoon/Tile/RSCookingTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSTableTile.h"


ARSTycoonWaiterCharacter::ARSTycoonWaiterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupLocation = CreateDefaultSubobject<USceneComponent>("FoodLocation");
	PickupLocation->SetupAttachment(RootComponent);
}

void ARSTycoonWaiterCharacter::Pickup(AActor* Actor)
{
	if (PickupActor)
	{
		RS_LOG_C("이미 음식을 들고 있습니다", FColor::Red);
		return;
	}

	PickupActor = Actor;

	Actor->AttachToComponent(PickupLocation, FAttachmentTransformRules::KeepWorldTransform);
	Actor->SetActorLocation(PickupLocation->GetComponentLocation());
}

AActor* ARSTycoonWaiterCharacter::Drop(FVector DropLocation)
{
	if (PickupActor == nullptr)
	{
		RS_LOG_C("들고있는 Actor가 없습니다", FColor::Red);
		return nullptr;
	}

	AActor* Temp = PickupActor;

	PickupActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PickupActor->SetActorLocation(DropLocation);
	PickupActor = nullptr;

	return Temp;
}


void ARSTycoonWaiterCharacter::InteractTarget(AActor* TargetActor)
{
	Super::InteractTarget(TargetActor);

	//테이블에 도착하면 상호작용
	//하는 상호작용 : 주문, 음식을 테이블에 내려놓음
	if (ARSTableTile* Tile = Cast<ARSTableTile>(TargetActor))
	{
		InteractTable(Tile);
	}

	//화구에 도착하면 상호작용
	//음식을 들음
	if (ARSCookingTile* Tile = Cast<ARSCookingTile>(TargetActor))
	{
		Tile->Interact(this);
	}
}

void ARSTycoonWaiterCharacter::BeginPlay()
{
	Super::BeginPlay();

	AAIController* AIController = Cast<AAIController>(GetController());
	check(AIController)

	AActor* TileMap = UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass());
	AIController->GetBlackboardComponent()->SetValueAsObject(FName(TEXT("TileMap")), Cast<ARSTileMap>(TileMap));
}

void ARSTycoonWaiterCharacter::InteractTable(ARSTableTile* Table)
{
	int32 OrderWaitIndex = Table->GetOrderWaitCustomerIndex();
	if (OrderWaitIndex != INDEX_NONE)
	{
		//오더를 받음
		AAIController* AIController = Cast<AAIController>(GetController());
		check(AIController)

		FName CustomerKey = TEXT("TargetCustomer");
		AIController->GetBlackboardComponent()->SetValueAsObject(CustomerKey, nullptr);

		Table->Interact(this);
	}
	else if (PickupActor)
	{
		if (ARSBaseFood* Food = Cast<ARSBaseFood>(PickupActor))
		{
			int32 FoodWaitIndex = Table->GetFoodWaitCustomerIndex(Food->Order);
			if (FoodWaitIndex != INDEX_NONE)
			{
				Table->Interact(this);
			}
		}
		else
		{
			RS_LOG_C("들고 있는 엑터가 음식이 아닙니다.", FColor::Red);
		}
	}
}
