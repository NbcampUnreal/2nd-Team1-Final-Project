// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonCustomerCharacter.h"

#include "AIController.h"
#include "CookFoodData.h"
#include "RSDataSubsystem.h"
#include "RSTycoonGameModeBase.h"
#include "RSTycoonPlayerController.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Tile/RSDoorTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSTableTile.h"

// Sets default values
ARSTycoonCustomerCharacter::ARSTycoonCustomerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARSTycoonCustomerCharacter::Sit(ARSTableTile* Table, const FTransform& SitTransform)
{
	State = ETycoonCustomerState::OrderWaiting;

	SitTableTile = Table;

	SetActorLocation(SitTransform.GetLocation());
	SetActorRotation(SitTransform.GetRotation());
}

void ARSTycoonCustomerCharacter::WaitFood()
{
	State = ETycoonCustomerState::FoodWaiting;

	FFoodOrder Order;
	Order.FoodKey = WantFoodKey;
	Order.Customer = this;
	
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	GameMode->AddOrder(Order);
}

void ARSTycoonCustomerCharacter::Eat()
{
	State = ETycoonCustomerState::Eat;

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &ARSTycoonCustomerCharacter::Leave, 5.f, false);
}

void ARSTycoonCustomerCharacter::InteractTarget(AActor* TargetActor)
{
	Super::InteractTarget(TargetActor);

	//의자에 닿으면 앉음
	if (ARSTableTile* TableTile = Cast<ARSTableTile>(TargetActor))
	{
		TableTile->Sit(this);
	}

	//문에 닿으면 삭제
	if (ARSDoorTile* DoorTile = Cast<ARSDoorTile>(TargetActor))
	{
		ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
		GameMode->RemoveCustomer(this);

		Destroy();
	}
}

void ARSTycoonCustomerCharacter::Leave()
{
	State = ETycoonCustomerState::Move;

	FCookFoodData* Data = GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Food->
	                                         FindRow<FCookFoodData>(WantFoodKey, TEXT("Get Price Of Food"));

	RS_LOG_F_C("손님이 식사를 다 했습니다, 골드 +%d", FColor::Yellow, Data->Price)

	ARSDoorTile* DoorTile = Cast<ARSDoorTile>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSDoorTile::StaticClass()));
	MoveToTarget(DoorTile->GetSpawnPoint(), DoorTile);

	GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>()->AddGold(Data->Price);
	
	OnFinishEat.Broadcast(this);
}
