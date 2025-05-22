// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonCustomerCharacter.h"

#include "AIController.h"
#include "RSTycoonGameModeBase.h"
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
	
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	GameMode->AddOrder({WantFoodKey, this});
}

void ARSTycoonCustomerCharacter::Eat()
{
	State = ETycoonCustomerState::Eat;
	
	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, [&]()
	{
		//나감
		State = ETycoonCustomerState::Move;
		RS_LOG_C("손님이 식사를 다 했습니다", FColor::Green)

		ARSDoorTile* DoorTile = Cast<ARSDoorTile>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSDoorTile::StaticClass()));
		MoveToTarget(DoorTile->GetSpawnPoint(), DoorTile);

		OnFinishEat.Broadcast(this);
	}, 5.f, false);
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
